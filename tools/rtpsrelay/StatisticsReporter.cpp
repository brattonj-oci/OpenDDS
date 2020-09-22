#include "StatisticsReporter.h"

namespace RtpsRelay {

StatisticsReporter::StatisticsReporter(size_t group_count)
  : priority_sources_()
  , source_groups_(group_count)
{
}
 
StatisticsReporter::~StatisticsReporter()
{
}

void StatisticsReporter::add_message_in(const ACE_INET_Addr& source, uint64_t byte_count)
{
}

void StatisticsReporter::add_message_out(const ACE_INET_Addr& dest, uint64_t byte_count)
{
}
  
void StatisticsReporter::update_fan_out(const ACE_INET_Addr& source, uint32_t value)
{
}

void StatisticsReporter::update_queue_size(uint64_t byte_count)
{
}

void StatisticsReporter::update_queue_latency(const Duration_t& updated_latency)
{
}

void StatisticsReporter::add_participant(const ACE_INET_Addr& remote_addr)
{
}

void StatisticsReporter::remove_participant(const ACE_INET_Addr& remove_addr)
{
}
  
void StatisticsReporter::handle_interval(const OpenDDS::DCPS::MonotonicTimePoint& time_now) {
  // Trigger all of the priority sources, and the next group
  priority_sources_.report(time_now);
  source_groups_.get_next_group().report(time_now);
}

#if 0
void StatisticsReporter::register_source(StatisticsSource_rch source)
{
  source_groups_.add_source(source);
}

void StatisticsReporter::register_priority_source(StatisticsSource_rch source)
{
  priority_sources_.add_source(source);
}
#endif

} // namespace RtpsRelay


