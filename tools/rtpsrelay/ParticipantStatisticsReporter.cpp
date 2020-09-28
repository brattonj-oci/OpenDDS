#include "ParticipantStatisticsReporter.h"

#include "lib/QosIndex.h"

#include <ace/Global_Macros.h>

#include <iostream>

namespace RtpsRelay {

ParticipantStatisticsReporter::ParticipantStatisticsReporter(ParticipantStatisticsDataWriter_ptr writer, bool log_stats)
  : writer_(writer)
  , log_stats_(log_stats)
{
}

ParticipantStatisticsReporter::~ParticipantStatisticsReporter()
{
}

void ParticipantStatisticsReporter::update_input_msgs(const OpenDDS::DCPS::RepoId& participant, size_t byte_count)
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  ParticipantStatistics& stats = get_stats(participant);
  stats._bytes_in += byte_count;
  ++stats._messages_in;
}

void ParticipantStatisticsReporter::update_output_msgs(const OpenDDS::DCPS::RepoId& participant, size_t byte_count)
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  ParticipantStatistics& stats = get_stats(participant);
  stats._bytes_out += byte_count;
  ++stats._messages_out;
}

void ParticipantStatisticsReporter::update_fan_out(const OpenDDS::DCPS::RepoId& participant, uint32_t value)
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  ParticipantStatistics& stats = get_stats(participant);
  stats._max_fan_out = std::max(stats._max_fan_out, value);
}

void ParticipantStatisticsReporter::report(const OpenDDS::DCPS::MonotonicTimePoint& time_now)
{
  // This is temporary - just to get the concept working
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  // Use this duration for any new participants
  const auto duration = time_now - last_report_time_;
  const auto base_duration = duration.to_dds_duration();
  last_report_time_ = time_now;

  ACE_TCHAR timestamp[OpenDDS::DCPS::AceTimestampSize];
  ACE::timestamp(timestamp, sizeof(timestamp) / sizeof(ACE_TCHAR));

  for (auto& kv : participant_stats_) {
    auto& last_time = kv.second.last_time;
    auto& stats = kv.second.stats;

    // Use the base interval if this participant has never published
    // stats before.  This will be wrong, but within 1 interval
    if (last_time.value().sec() == 0) {
      stats._interval._sec = base_duration.sec;
      stats._interval._nanosec = base_duration.nanosec;
    } else {
      const auto time_diff = time_now - last_time;
      const auto new_duration = time_diff.to_dds_duration();
      stats._interval._sec = new_duration.sec;
      stats._interval._nanosec = new_duration.nanosec;
    }
    last_time = time_now;

    // Update the timing data for this participant
    if (log_stats_) {
      std::cout << timestamp << ' '
                << "participant_guid=" << kv.first << ' '
                << "interval=" << stats.interval().sec() << '.' << stats.interval().nanosec() << ' '
                << "messages_in=" << stats.messages_in() << ' '
                << "bytes_in=" << stats.bytes_in() << ' '
                << "messages_out=" << stats.messages_out() << ' '
                << "bytes_out=" << stats.bytes_out() << ' '
                << std::endl;
    }

    if (writer_) {
      const auto ret = writer_->write(stats, DDS::HANDLE_NIL);
      if (ret != DDS::RETCODE_OK) {
        ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) %N:%l ERROR: writing participant %C statistics\n"), kv.first.c_str()));
      }
    }

    reset_participant_stats(stats);
  }
}

void ParticipantStatisticsReporter::reset_stats()
{
  // Done in report
}

ParticipantStatistics& ParticipantStatisticsReporter::get_stats(const OpenDDS::DCPS::RepoId& participant)
{
  // This function should always be called with the mutex already held
  std::string id_str = guid_to_string(participant);
  auto iPart = participant_stats_.find(id_str);
  if (iPart != participant_stats_.end()) {
    return iPart->second.stats;
  }

  // Must be a new participant
  ParticipantStatsNode new_stats;
  reset_participant_stats(new_stats.stats);
  participant_stats_[id_str] = new_stats;
  return participant_stats_[id_str].stats;
}

void ParticipantStatisticsReporter::reset_participant_stats(ParticipantStatistics& stats)
{
  // This should be called with the guard in place
  stats._bytes_in = 0;
  stats._messages_in = 0;
  stats._bytes_out = 0;
  stats._messages_out = 0;
  stats._max_fan_out = 0;
  stats._interval.sec(0);
  stats._interval.nanosec(0);
}

void ParticipantStatisticsReporter::remove_participant(const OpenDDS::DCPS::RepoId& guid)
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  std::string id_str = guid_to_string(guid);
  participant_stats_.find(id_str);
}

} // namespace RtpsRelay


