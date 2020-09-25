#include "ParticipantStatisticsReporterBase.h"


namespace RtpsRelay {

ParticipantStatisticsReporterBase::ParticipantStatisticsReporterBase()
{
}
 
ParticipantStatisticsReporterBase::~ParticipantStatisticsReporterBase()
{
}

void ParticipantStatisticsReporterBase::update_input_msgs(const GuidAddr& participant, size_t byte_count)
{
  ACE_UNUSED_ARG(participant);
  ACE_UNUSED_ARG(byte_count);
}

void ParticipantStatisticsReporterBase::update_output_msgs(const GuidAddr& participant, size_t byte_count)
{
  ACE_UNUSED_ARG(participant);
  ACE_UNUSED_ARG(byte_count);
}
  
void ParticipantStatisticsReporterBase::update_fan_out(const GuidAddr& participant, uint32_t value)
{
  ACE_UNUSED_ARG(participant);
  ACE_UNUSED_ARG(value);
}

void ParticipantStatisticsReporterBase::report(const OpenDDS::DCPS::MonotonicTimePoint& time_now)
{
  ACE_UNUSED_ARG(time_now);
}

void ParticipantStatisticsReporterBase::reset_stats()
{
}

} // namespace RtpsRelay


