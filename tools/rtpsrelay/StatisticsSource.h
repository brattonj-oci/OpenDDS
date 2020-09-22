#ifndef RTPSRELAY_STATISTICS_SOURCE_H_
#define RTPSRELAY_STATISTICS_SOURCE_H_

#include "dds/DCPS/RcObject.h"
#include "dds/DCPS/RcHandle_T.h"

namespace RtpsRelay {

class StatisticsSource : public OpenDDS::DCPS::RcObject {
public:

  enum ReportMode {
    LOG             = 0,
    PUBLISH         = 1,
    LOG_AND_PUBLISH = 2
  };
  
  virtual ~StatisticsSource() {}
  
  void report() {
    if (ReportMode::LOG == mode_) {
      log_stats();
    } else if (ReportMode::PUBLISH == mode_) {
      publish_stats();
    } else {
      log_stats();
      publish_stats();
    }
    reset_stats();
  }
  
  virtual void reset_stats() = 0;
protected:

  explicit StatisticsSource(ReportMode mode)
  : mode_(mode)
  {
  }
  
private:
  virtual void publish_stats() = 0;
  virtual void log_stats() = 0;
  
  ReportMode mode_;
};

typedef OpenDDS::DCPS::RcHandle<StatisticsSource> StatisticsSource_rch;


} // namespace RtpsRelay

#endif // RTPSRELAY_STATISTICS_SOURCE_H_
