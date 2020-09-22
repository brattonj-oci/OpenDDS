#ifndef RTPSRELAY_HANDLER_STATISTICS_SOURCE_H_
#define RTPSRELAY_HANDLER_STATISTICS_SOURCE_H_

#include "StatisticsSource.h"

#include "../lib/RelayTypeSupportImpl.h"

namespace RtpsRelay {

class HandlerStatisticsSource : public StatisticsSource {
public:

  explicit HandlerStatisticsSource(const OpenDDS::DCPS::RepoId& app_participant_guid, 
                                   StatisticsSource::ReportMode mode);

  virtual ~HandlerStatisticsSource();
  
  // Functions to update the statistics
  void add_message_in(uint64_t byte_count);
  void add_message_out(uint64_t byte_count);

  void update_fan_out(uint32_t value);
  void update_queue_size(uint32_t value);
  
  //void active_participtans
  void update_local_active_participants(uint32_t value);
  void update_queue_latency(const Duration_t& updated_latency);

protected:

   
private:

  void publish_stats() override;
  void log_stats() override;
  void reset_stats() override;
  
  HandlerStatistics stats_;
  std::string particpant_name_;
};

typedef OpenDDS::DCPS::RcHandle<HandlerStatisticsSource> HandlerStatisticsSource_rch;
}

#endif // RTPSRELAY_HANDLER_STATISTICS_SOURCE_H_
