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

void ParticipantStatisticsReporter::update_queue_size(const GuidAddr& participant, uint32_t value)
{
  // ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  // ParticipantStatistics& stats = get_stats(participant_guid);
  // stats._max_queue_size = std::max(stats._max_queue_size, value);
}

void ParticipantStatisticsReporter::report(const OpenDDS::DCPS::MonotonicTimePoint& time_now)
{
  //HandlerStatistics cached_stats;
  //copy_stats(cached_stats);
  //log_stats(cached_stats);

  // This is temporary - just to get the concept working
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  for (auto& kv : participant_stats_) {
    auto& ga = kv.first;
    auto& stats = kv.second;
    std::cout << "guid=" << guid_to_string(ga.guid) << ' '
              << "addr=" << addr_to_string(ga.address) << ' '
              << "messages_in=" << stats.messages_in() << ' '
              << "bytes_in=" << stats.bytes_in() << ' '
              << "messages_out=" << stats.messages_out() << ' '
              << "bytes_out=" << stats.bytes_out() << ' '
              << std::endl;

    reset_participant_stats(stats);
  }
}

// void RelayHandlerStatistics::log_stats(HandlerStatistics& hs)
// {
//   int timestamp = 0; // TBD
//   std::cout << timestamp << ' '
//             << "application_participant_guid=" << particpant_name_ << ' '
//             << "name=\"" << hs.name() << "\" "
//             << "interval=" << hs.interval().sec() << '.' << hs.interval().nanosec() << ' '
//             << "messages_in=" << hs.messages_in() << ' '
//             << "bytes_in=" << hs.bytes_in() << ' '
//             << "messages_out=" << hs.messages_out() << ' '
//             << "bytes_out=" << hs.bytes_out() << ' '
//             << "max_fan_out=" << hs.max_fan_out() << ' '
//             << "max_queue_size=" << hs.max_queue_size() << ' '
//             << "max_queue_latency=" << hs.max_queue_latency().sec() << '.' << hs.max_queue_latency().nanosec() << ' '
//             << "local_active_participants=" << hs.local_active_participants()
//             << std::endl;
// }

void ParticipantStatisticsReporter::reset_stats()
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);
  //
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
  //stats._max_queue_size = 0;
  stats._max_queue_latency._sec = 0;
  stats._max_queue_latency._nanosec = 0;
}

// void RelayHandlerStatistics::copy_stats(HandlerStatistics& to)
// {
//   ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

//   to.application_participant_guid(handler_stats_.application_participant_guid());
//   to.name(handler_stats_.name());
//   to._bytes_in = handler_stats_._bytes_in;
//   to._messages_in = handler_stats_._messages_in;
//   to._bytes_out = handler_stats_._bytes_out;
//   to._messages_out = handler_stats_._messages_out;
//   to._max_fan_out = handler_stats_._max_fan_out;
//   to._max_queue_size = handler_stats_._max_queue_size;
//   to._max_queue_latency._sec = handler_stats_._max_queue_latency._sec;
//   to._max_queue_latency._nanosec = handler_stats_._max_queue_latency._nanosec;
// }

} // namespace RtpsRelay


