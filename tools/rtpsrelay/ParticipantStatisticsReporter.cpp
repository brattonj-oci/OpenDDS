#include "ParticipantStatisticsReporter.h"

#include "lib/QosIndex.h"

#include <ace/Global_Macros.h>

#include <iostream>

namespace RtpsRelay {

ParticipantStatisticsReporter::ParticipantStatisticsReporter()
{
}
 
ParticipantStatisticsReporter::~ParticipantStatisticsReporter()
{
}

void ParticipantStatisticsReporter::update_input_msgs(const GuidAddr& participant, size_t byte_count)
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  ParticipantStatistics& stats = get_stats(participant);
  stats._bytes_in += byte_count;
  ++stats._messages_in;
}

void ParticipantStatisticsReporter::update_output_msgs(const GuidAddr& participant, size_t byte_count)
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  ParticipantStatistics& stats = get_stats(participant);
  stats._bytes_out += byte_count;
  ++stats._messages_out;
}
  
void ParticipantStatisticsReporter::update_fan_out(const GuidAddr& participant, uint32_t value)
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  ParticipantStatistics& stats = get_stats(participant);
  stats._max_fan_out = std::max(stats._max_fan_out, value);
}

void ParticipantStatisticsReporter::report(const OpenDDS::DCPS::MonotonicTimePoint& time_now)
{
  // This is temporary - just to get the concept working
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  // Save off timing information and publish and/or log the data
  //const auto duration = time_now - last_report_time_;
  //const auto dds_duration = duration.to_dds_duration();
  //last_report_time_ = time_now;

  ACE_TCHAR timestamp[OpenDDS::DCPS::AceTimestampSize];
  ACE::timestamp(timestamp, sizeof(timestamp) / sizeof(ACE_TCHAR));

  //handler_stats_._interval._sec = dds_duration.sec;
  //handler_stats_._interval._nanosec = dds_duration.nanosec;

  for (auto& kv : participant_stats_) {
    auto& ga = kv.first;
    auto& stats = kv.second;

    //const auto duration = time_now - last_report_time_;
    //const auto dds_duration = duration.to_dds_duration();
    //last_report_time_ = time_now;

    // Update the timing data for this participant
    std::cout << timestamp << ' '
              << "participant_guid=" << guid_to_string(ga.guid) << ' '
              << "participant_addr=" << addr_to_string(ga.address) << ' '
              << "interval=" << 
              << "messages_in=" << stats.messages_in() << ' '
              << "bytes_in=" << stats.bytes_in() << ' '
              << "messages_out=" << stats.messages_out() << ' '
              << "bytes_out=" << stats.bytes_out() << ' '
              << std::endl;

    reset_participant_stats(stats);
  }
}

void ParticipantStatisticsReporter::reset_stats()
{
  // Done in report
}

ParticipantStatistics& ParticipantStatisticsReporter::get_stats(const GuidAddr& participant) 
{
  // This function should always be called with the mutex already held
  auto iPart = participant_stats_.find(participant);
  if (iPart != participant_stats_.end()) {
    return iPart->second;
  }

  // Must be a new participant
  ParticipantStatistics new_stats;
  reset_participant_stats(new_stats);
  participant_stats_[participant] = new_stats;
  return participant_stats_[participant];
}

void ParticipantStatisticsReporter::reset_participant_stats(ParticipantStatistics& stats)
{
  // This should be called with the guard in place
  stats._bytes_in = 0;
  stats._messages_in = 0;
  stats._bytes_out = 0;
  stats._messages_out = 0;
  stats._max_fan_out = 0;
}

} // namespace RtpsRelay


