#include "HandlerStatisticsSource.h"
#include "utility.h"

#include "lib/RelayTypeSupportImpl.h"
#include "lib/QosIndex.h"

#include <algorithm>
#include <iostream>

namespace RtpsRelay {

// Add support for std::max to Duration_t
bool
operator<(const Duration_t& x, const Duration_t& y)
{
  if (x.sec() != y.sec()) {
    return x.sec() < y.sec();
  }
  return x.nanosec() < y.nanosec();
}


HandlerStatisticsSource::HandlerStatisticsSource(const OpenDDS::DCPS::RepoId& app_participant_guid,
                                                StatisticsSource::ReportMode mode)
  : StatisticsSource(mode)
{
  stats_.application_participant_guid(repoid_to_guid(app_participant_guid));
  particpant_name_ = guid_to_string(guid_to_repoid(stats_.application_participant_guid()));
}

HandlerStatisticsSource::~HandlerStatisticsSource()
{
}
  
void HandlerStatisticsSource::publish_stats()
{
}

void HandlerStatisticsSource::log_stats()
{
  int timestamp = 0; // TBD
  std::cout << timestamp << ' '
            << "application_participant_guid=" << particpant_name_ << ' '
            << "name=\"" << stats_.name() << "\" "
            << "interval=" << stats_.interval().sec() << '.' << stats_.interval().nanosec() << ' '
            << "messages_in=" << stats_.messages_in() << ' '
            << "bytes_in=" << stats_.bytes_in() << ' '
            << "messages_out=" << stats_.messages_out() << ' '
            << "bytes_out=" << stats_.bytes_out() << ' '
            << "max_man_out=" << stats_.max_fan_out() << ' '
            << "max_queue_size=" << stats_.max_queue_size() << ' '
            << "max_queue_latency=" << stats_.max_queue_latency().sec() << '.' << stats_.max_queue_latency().nanosec() << ' '
            << "local_active_participants=" << stats_.local_active_participants()
            << std::endl;
}

void HandlerStatisticsSource::reset_stats()
{
  stats_._bytes_in = 0;
  stats_._messages_in = 0;
  stats_._bytes_out = 0;
  stats_._messages_out = 0;
  stats_._max_fan_out = 0;
  stats_._max_queue_size = 0;
  stats_._max_queue_latency._sec = 0;
  stats_._max_queue_latency._nanosec = 0;
}

void HandlerStatisticsSource::add_message_in(uint64_t byte_count)
{
  stats_._bytes_in += byte_count;
  ++stats_._messages_in;
}

void HandlerStatisticsSource::add_message_out(uint64_t byte_count)
{
  stats_._bytes_out += byte_count;
  ++stats_._messages_out;
}

void HandlerStatisticsSource::update_fan_out(uint32_t value)
{
  stats_._max_fan_out = std::max(stats_._max_fan_out, value);
}

void HandlerStatisticsSource::update_queue_size(uint32_t value)
{
  stats_._max_fan_out = std::max(stats_._max_fan_out, value);
}

void HandlerStatisticsSource::update_local_active_participants(uint32_t value)
{
  stats_.local_active_participants(value);
}

void HandlerStatisticsSource::update_queue_latency(const Duration_t& updated_latency)
{
  stats_._max_queue_latency = std::max(stats_._max_queue_latency, updated_latency);
}

} // namespace RtpsRelay
