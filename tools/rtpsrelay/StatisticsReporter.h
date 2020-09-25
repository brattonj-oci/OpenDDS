#ifndef RTPSRELAY_STATISTICS_REPORTER_H_
#define RTPSRELAY_STATISTICS_REPORTER_H_

#include "dds/DCPS/RcObject.h"
#include "dds/DCPS/RcHandle_T.h"
#include "dds/DCPS/TimeTypes.h"


namespace RtpsRelay {

class StatisticsReporter : public OpenDDS::DCPS::RcObject {
public:
  virtual ~StatisticsReporter() {}

  virtual void report(const OpenDDS::DCPS::MonotonicTimePoint& time_now) = 0;
  virtual void reset_stats();
protected:

  explicit StatisticsReporter() {}
};

typedef OpenDDS::DCPS::RcHandle<StatisticsReporter> StatisticsReporter_rch;

}

#endif // RTPSRELAY_STATISTICS_REPORTER_H_
