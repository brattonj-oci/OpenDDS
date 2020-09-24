#ifndef RTPSRELAY_PARTICIPANT_STATISTICS_REPORTER_IMPL_H_
#define RTPSRELAY_PARTICIPANT_STATISTICS_REPORTER_IMPL_H_

#include "StatisticsReporter.h"
#include "utility.h"
#include "lib/RelayTypeSupportImpl.h"

#include "dds/DCPS/RcHandle_T.h"
#include "dds/DCPS/TimeTypes.h"

#include <ace/Thread_Mutex.h>
#include <ace/INET_Addr.h>

#include <map>

namespace RtpsRelay {

class ParticipantStatisticsReporter : public StatisticsReporter {
public:
  
  explicit ParticipantStatisticsReporter();

  virtual ~ParticipantStatisticsReporter();
  
  void update_input_msgs(const GuidAddr& participant, size_t byte_count);

  void update_output_msgs(const GuidAddr& participant, size_t byte_count);

  void update_fan_out(const GuidAddr& participant, uint32_t value);
  void update_queue_size(const GuidAddr& participant, uint32_t value);
  void update_queue_latency(const GuidAddr& participant, const Duration_t& updated_latency);
     
  void report(const OpenDDS::DCPS::MonotonicTimePoint& time_now);
  void reset_stats();

private:

  //void log_stats(HandlerStatistics& to);
  //void copy_stats(HandlerStatistics& to);
  std::map<GuidAddr, ParticipantStatistics> participant_stats_;
  
  ParticipantStatistics& get_stats(const GuidAddr& participant);
  void reset_participant_stats(ParticipantStatistics& stats);

  ACE_Thread_Mutex stats_mutex_;
};

typedef OpenDDS::DCPS::RcHandle<ParticipantStatisticsReporter> ParticipantStatisticsReporter_rch;
}

#endif // RTPSRELAY_PARTICIPANT_STATISTICS_REPORTER_IMPL_H_
