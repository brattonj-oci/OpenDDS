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

RelayHandlerStatistics::RelayHandlerStatistics(
  const OpenDDS::DCPS::RepoId& participant_guid,
  const std::string& name,
  HandlerStatisticsDataWriter_ptr writer)
  : writer_(writer)
{
  handler_stats_.application_participant_guid(repoid_to_guid(participant_guid));
  handler_stats_.name(name);
  particpant_name_ = guid_to_string(
    guid_to_repoid(handler_stats_.application_participant_guid()));

}
 
RelayHandlerStatistics::~RelayHandlerStatistics()
{
}

void RelayHandlerStatistics::update_input_msgs(size_t byte_count)
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  handler_stats_._bytes_in += byte_count;
  ++handler_stats_._messages_in;
}

void RelayHandlerStatistics::update_output_msgs(size_t byte_count)
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  handler_stats_._bytes_out += byte_count;
  ++handler_stats_._messages_out;
}
  
void RelayHandlerStatistics::update_fan_out(uint32_t value)
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

void RelayHandlerStatistics::update_local_participants(uint32_t num_participants)
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  handler_stats_._local_active_participants = num_participants;
}

void RelayHandlerStatistics::report_error()
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  ++handler_stats_._error_count;
}

void RelayHandlerStatistics::governor_active()
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  ++handler_stats_._governor_count;  
}

void RelayHandlerStatistics::report(const OpenDDS::DCPS::MonotonicTimePoint& time_now)
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  // Save off timing information and publish and/or log the data
  const auto duration = time_now - last_report_time_;
  const auto dds_duration = duration.to_dds_duration();
  last_report_time_ = time_now;

  handler_stats_._interval._sec = dds_duration.sec;
  handler_stats_._interval._nanosec = dds_duration.nanosec;
  
  if (writer_) {
    const auto ret = writer_->write(handler_stats_, DDS::HANDLE_NIL);
    if (ret != DDS::RETCODE_OK) {
      ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) %N:%l ERROR: RelayHandler::handle_timeout %C failed to write handler statistics\n"), handler_stats_.name().c_str()));
    }
  }
  log_stats(handler_stats_);
}

void RelayHandlerStatistics::log_stats(HandlerStatistics& hs)
{
  int timestamp = 0; // TBD
  std::cout << timestamp << ' '
            << "application_participant_guid=" << particpant_name_ << ' '
            << "name=\"" << hs.name() << "\" "
            << "interval=" << hs.interval().sec() << '.' << hs.interval().nanosec() << ' '
            << "messages_in=" << hs.messages_in() << ' '
            << "bytes_in=" << hs.bytes_in() << ' '
            << "messages_out=" << hs.messages_out() << ' '
            << "bytes_out=" << hs.bytes_out() << ' '
            << "max_fan_out=" << hs.max_fan_out() << ' '
            << "max_queue_size=" << hs.max_queue_size() << ' '
            << "max_queue_latency=" << hs.max_queue_latency().sec() << '.' << hs.max_queue_latency().nanosec() << ' '
            << "local_active_participants=" << hs.local_active_participants() << ' '
            << "error_count=" << hs.error_count() << ' '
            << "governor_count=" << hs.governor_count()
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

void RelayHandlerStatistics::copy_stats(HandlerStatistics& to)
{
  ACE_GUARD(ACE_Thread_Mutex, g, stats_mutex_);

  to.application_participant_guid(handler_stats_.application_participant_guid());
  to.name(handler_stats_.name());
  to._bytes_in = handler_stats_._bytes_in;
  to._messages_in = handler_stats_._messages_in;
  to._bytes_out = handler_stats_._bytes_out;
  to._messages_out = handler_stats_._messages_out;
  to._max_fan_out = handler_stats_._max_fan_out;
  to._max_queue_size = handler_stats_._max_queue_size;
  to._max_queue_latency._sec = handler_stats_._max_queue_latency._sec;
  to._max_queue_latency._nanosec = handler_stats_._max_queue_latency._nanosec;
}

} // namespace RtpsRelay


