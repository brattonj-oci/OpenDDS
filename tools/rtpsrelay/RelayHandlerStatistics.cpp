#include "RelayHandlerStatistics.h"
#include "utility.h"

#include "lib/QosIndex.h"

#include <ace/Global_Macros.h>

#include <iostream>

namespace RtpsRelay {

// Add support for std::max to Duration_t
bool operator<(const Duration_t& x, const Duration_t& y)
{
  if (x.sec() != y.sec()) {
    return x.sec() < y.sec();
  }
  return x.nanosec() < y.nanosec();
}

RelayHandlerStatistics::RelayHandlerStatistics(const OpenDDS::DCPS::RepoId& participant_guid,
                                               const std::string& name)
{
  handler_stats_.application_participant_guid(repoid_to_guid(participant_guid));
  handler_stats_.name(name);
  particpant_name_ = guid_to_string(
    guid_to_repoid(handler_stats_.application_participant_guid()));
}
 
RelayHandlerStatistics::~RelayHandlerStatistics()
{
}

void RelayHandlerStatistics::process_input_msg(const ACE_INET_Addr& source, uint32_t byte_count)
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  handler_stats_._bytes_in += byte_count;
  ++handler_stats_._messages_in;
}

void RelayHandlerStatistics::process_output_msg(const ACE_INET_Addr& dest, uint32_t byte_count)
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  handler_stats_._bytes_out += byte_count;
  ++handler_stats_._messages_out;
}
  
void RelayHandlerStatistics::update_fan_out(const ACE_INET_Addr& source, uint32_t value)
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  handler_stats_._max_fan_out = std::max(handler_stats_._max_fan_out, value);
}

void RelayHandlerStatistics::update_queue_size(uint32_t value)
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  handler_stats_._max_queue_size = std::max(handler_stats_._max_queue_size, value);
}

void RelayHandlerStatistics::update_queue_latency(const Duration_t& updated_latency)
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  handler_stats_._max_queue_latency = std::max(handler_stats_._max_queue_latency, updated_latency);
}

void RelayHandlerStatistics::add_participant(const ACE_INET_Addr& remote_addr)
{
  // ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);
}

void RelayHandlerStatistics::remove_participant(const ACE_INET_Addr& remove_addr)
{
  // ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);
}
  
// void RelayHandlerStatistics::handle_interval(const OpenDDS::DCPS::MonotonicTimePoint& time_now) {
//   // Trigger all of the priority sources, and the next group
//   priority_sources_.report(time_now);
//   source_groups_.get_next_group().report(time_now);
// }

void RelayHandlerStatistics::log_stats(const OpenDDS::DCPS::MonotonicTimePoint& time_now)
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  int timestamp = 0; // TBD
  std::cout << timestamp << ' '
            << "application_participant_guid=" << particpant_name_ << ' '
            << "name=\"" << handler_stats_.name() << "\" "
            << "interval=" << handler_stats_.interval().sec() << '.' << handler_stats_.interval().nanosec() << ' '
            << "messages_in=" << handler_stats_.messages_in() << ' '
            << "bytes_in=" << handler_stats_.bytes_in() << ' '
            << "messages_out=" << handler_stats_.messages_out() << ' '
            << "bytes_out=" << handler_stats_.bytes_out() << ' '
            << "max_fan_out=" << handler_stats_.max_fan_out() << ' '
            << "max_queue_size=" << handler_stats_.max_queue_size() << ' '
            << "max_queue_latency=" << handler_stats_.max_queue_latency().sec() << '.' << handler_stats_.max_queue_latency().nanosec() << ' '
            << "local_active_participants=" << handler_stats_.local_active_participants()
            << std::endl;
}

void RelayHandlerStatistics::reset_stats()
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);
  
  handler_stats_._bytes_in = 0;
  handler_stats_._messages_in = 0;
  handler_stats_._bytes_out = 0;
  handler_stats_._messages_out = 0;
  handler_stats_._max_fan_out = 0;
  handler_stats_._max_queue_size = 0;
  handler_stats_._max_queue_latency._sec = 0;
  handler_stats_._max_queue_latency._nanosec = 0;
}


} // namespace RtpsRelay


