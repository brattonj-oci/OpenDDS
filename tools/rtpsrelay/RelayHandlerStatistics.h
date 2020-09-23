#ifndef RTPSRELAY_STATISTICS_REPORTER_H_
#define RTPSRELAY_STATISTICS_REPORTER_H_

#include "lib/RelayTypeSupportImpl.h"

//#include "dds/DCPS/RcObject_T.h"
//#include "dds/DCPS/RcHandle_T.h"
//#include "StatisticsSource.h"

#include "dds/DCPS/RcObject.h"
#include "dds/DCPS/RcHandle_T.h"
#include "dds/DCPS/TimeTypes.h"

#include "ace/INET_Addr.h"
#include <ace/Thread_Mutex.h>

#include <vector>

// Notes:
//
// What we want here is a way to stagger publications so that not
// everything is publishing all the time.  MOre details needed on this.
//
// The following information will be published by this class
// 1. Participant statistics
// 2. Domain Statistics
// 3. Handler Statistics
// 4. Governor statistics (TBD)
//
// Errors will be sent when they happen
//
// The stats interval will be a full period, where all stats are
// updated at least once per interval
//
// Within the stats interval, there will be frames where subsets
// are published.  The "minor" frame will be period/groups

namespace RtpsRelay {

// class SourceGroup {
// public:
//   typedef std::vector<StatisticsSource_rch> SourceList;
  
//   explicit SourceGroup() : sources_() {}
//   virtual ~SourceGroup() { sources_.clear(); }
    
//   void add_source(StatisticsSource_rch source) {
//     sources_.push_back(source);
//   }
  
//   void report(const OpenDDS::DCPS::MonotonicTimePoint& time_now) {
//     for (auto& source : sources_) {
//       source->report();
//     }
//   }
  
// private:
//   SourceList sources_;
// };

// class CircularGroupList {
  
// public:
//   explicit CircularGroupList(size_t group_count) 
//     : source_groups_(group_count)
//     , read_index_(0ULL)
//     , write_index_(0ULL)
//     {}
    
//   virtual ~CircularGroupList() {}
  
//   void add_source(StatisticsSource_rch source)
//   {
//     source_groups_[write_index_++].add_source(source);
//     write_index_ = write_index_ % source_groups_.size();
//   }
  
//   SourceGroup& get_next_group()
//   {
//     SourceGroup& next_group = source_groups_[read_index_++];
//     read_index_ = read_index_ % source_groups_.size();
    
//     return next_group;
//   }

// private:
//   std::vector<SourceGroup> source_groups_;
//   size_t read_index_;
//   size_t write_index_;
// };

  
class RelayHandlerStatistics {
public:
  
  explicit RelayHandlerStatistics(const OpenDDS::DCPS::RepoId& participant_guid,
                                  const std::string& name);
  virtual ~RelayHandlerStatistics();
  

  void process_input_msg(const ACE_INET_Addr& source, uint32_t byte_count);
  void process_output_msg(const ACE_INET_Addr& dest, uint32_t byte_count);

  void update_fan_out(const ACE_INET_Addr& source, uint32_t value);

  void update_queue_size(uint32_t value);
  void update_queue_latency(const Duration_t& updated_latency);
  
  void add_participant(const ACE_INET_Addr& remote_addr);
  void remove_participant(const ACE_INET_Addr& remove_addr);
  
  // Timer function    
  void log_stats(const OpenDDS::DCPS::MonotonicTimePoint& time_now);

  void reset_stats();
private:

  //std::map<ACE_INET_Addr, ParticipantStatistics> participant_statistics_;
  
  HandlerStatistics handler_stats_;

  std::string particpant_name_;

  ACE_Thread_Mutex stats_mutex_;
};

}

#endif // RTPSRELAY_STATISTICS_REPORTER_H_
