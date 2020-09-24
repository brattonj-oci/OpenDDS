#ifndef RTPSRELAY_RELAY_HANDLER_STATISTICS_IMPL_H_
#define RTPSRELAY_RELAY_HANDLER_STATISTICS_IMPL_H_

#include "StatisticsReporter.h"
#include "lib/RelayTypeSupportImpl.h"

#include "dds/DCPS/RcObject.h"
#include "dds/DCPS/RcHandle_T.h"
#include "dds/DCPS/TimeTypes.h"

#include <ace/Thread_Mutex.h>

namespace RtpsRelay {

class RelayHandlerStatistics : public StatisticsReporter {
public:
  
  explicit RelayHandlerStatistics(const OpenDDS::DCPS::RepoId& participant_guid,
                                  const std::string& name);

  virtual ~RelayHandlerStatistics();
  
  void update_input_msgs(size_t byte_count);
  void update_output_msgs(size_t byte_count);

  void update_fan_out(uint32_t value);
  void update_queue_size(uint32_t value);
  void update_queue_latency(const Duration_t& updated_latency);
  
  void update_local_participants(uint32_t num_participants);
   
  void report(const OpenDDS::DCPS::MonotonicTimePoint& time_now);
  void reset_stats();

private:

  void log_stats(HandlerStatistics& to);
  void copy_stats(HandlerStatistics& to);

  HandlerStatistics handler_stats_;

  std::string particpant_name_;

  ACE_Thread_Mutex stats_mutex_;
};

typedef OpenDDS::DCPS::RcHandle<RelayHandlerStatistics> RelayHandlerStatistics_rch;
}

#endif // RTPSRELAY_RELAY_HANDLER_STATISTICS_IMPL_H_
