#ifndef RTPSRELAY_PARTICIPANT_STATISTICS_REPORTER_H_
#define RTPSRELAY_PARTICIPANT_STATISTICS_REPORTER_H_

#include "ParticipantStatisticsReporterBase.h"

#include <ace/Thread_Mutex.h>

#include <map>

namespace RtpsRelay {

class ParticipantStatisticsReporter : public ParticipantStatisticsReporterBase {
public:
  
  explicit ParticipantStatisticsReporter();

  virtual ~ParticipantStatisticsReporter();
  
  void update_input_msgs(const GuidAddr& participant, size_t byte_count) override;

  void update_output_msgs(const GuidAddr& participant, size_t byte_count) override;

  void update_fan_out(const GuidAddr& participant, uint32_t value) override;
  void update_queue_size(const GuidAddr& participant, uint32_t value) override;
   
  void report(const OpenDDS::DCPS::MonotonicTimePoint& time_now) override;
  void reset_stats() override;

private:

  std::map<GuidAddr, ParticipantStatistics> participant_stats_;
  
  ParticipantStatistics& get_stats(const GuidAddr& participant);
  void reset_participant_stats(ParticipantStatistics& stats);

  ACE_Thread_Mutex stats_mutex_;
};

typedef OpenDDS::DCPS::RcHandle<ParticipantStatisticsReporter> ParticipantStatisticsReporter_rch;
}

#endif // RTPSRELAY_PARTICIPANT_STATISTICS_REPORTER_H_
