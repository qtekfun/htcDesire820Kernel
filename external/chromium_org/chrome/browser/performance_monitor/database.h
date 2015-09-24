// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PERFORMANCE_MONITOR_DATABASE_H_
#define CHROME_BROWSER_PERFORMANCE_MONITOR_DATABASE_H_

#include <set>
#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/performance_monitor/constants.h"
#include "chrome/browser/performance_monitor/event.h"
#include "chrome/browser/performance_monitor/metric.h"
#include "third_party/leveldatabase/src/include/leveldb/db.h"

namespace performance_monitor {

struct TimeRange {
  TimeRange();
  TimeRange(base::Time start_time, base::Time end_time);
  ~TimeRange();

  base::Time start;
  base::Time end;
};

class KeyBuilder;
class DatabaseTestHelper;

class Database {
 public:
  typedef std::set<EventType> EventTypeSet;
  typedef std::vector<linked_ptr<Event> > EventVector;
  typedef std::set<MetricType> MetricTypeSet;
  typedef std::vector<Metric> MetricVector;
  typedef std::map<std::string, linked_ptr<MetricVector> > MetricVectorMap;

  static const char kDatabaseSequenceToken[];

  
  
  class Clock {
   public:
    Clock() {}
    virtual ~Clock() {}
    virtual base::Time GetTime() = 0;
  };

  virtual ~Database();

  static scoped_ptr<Database> Create(base::FilePath path);

  
  
  bool AddStateValue(const std::string& key, const std::string& value);

  std::string GetStateValue(const std::string& key);

  
  bool AddEvent(const Event& event);

  
  
  EventVector GetEvents(EventType type,
                        const base::Time& start,
                        const base::Time& end);

  EventVector GetEvents(const base::Time& start, const base::Time& end) {
    return GetEvents(EVENT_UNDEFINED, start, end);
  }

  EventVector GetEvents(EventType type) {
    return GetEvents(type, base::Time(), clock_->GetTime());
  }

  EventVector GetEvents() {
    return GetEvents(EVENT_UNDEFINED, base::Time(), clock_->GetTime());
  }

  EventTypeSet GetEventTypes(const base::Time& start, const base::Time& end);

  EventTypeSet GetEventTypes() {
    return GetEventTypes(base::Time(), clock_->GetTime());
  }

  
  bool AddMetric(const std::string& activity, const Metric& metric);

  bool AddMetric(const Metric& metric) {
    return AddMetric(kProcessChromeAggregate, metric);
  }

  
  
  MetricTypeSet GetActiveMetrics(const base::Time& start,
                                 const base::Time& end);

  
  std::set<std::string> GetActiveActivities(MetricType metric_type,
                                            const base::Time& start);

  
  double GetMaxStatsForActivityAndMetric(const std::string& activity,
                                         MetricType metric_type);
  double GetMaxStatsForActivityAndMetric(MetricType metric_type) {
    return GetMaxStatsForActivityAndMetric(kProcessChromeAggregate,
                                           metric_type);
  }

  
  
  bool GetRecentStatsForActivityAndMetric(const std::string& activity,
                                          MetricType metric_type,
                                          Metric* metric);

  bool GetRecentStatsForActivityAndMetric(MetricType metric_type,
                                           Metric* metric) {
    return GetRecentStatsForActivityAndMetric(kProcessChromeAggregate,
                                              metric_type,
                                              metric);
  }

  
  scoped_ptr<MetricVector> GetStatsForActivityAndMetric(
      const std::string& activity,
      MetricType metric_type,
      const base::Time& start,
      const base::Time& end);

  scoped_ptr<MetricVector> GetStatsForActivityAndMetric(
      MetricType metric_type, const base::Time& start, const base::Time& end) {
    return GetStatsForActivityAndMetric(kProcessChromeAggregate, metric_type,
                                        start, end);
  }

  scoped_ptr<MetricVector> GetStatsForActivityAndMetric(
      const std::string& activity, MetricType metric_type) {
    return GetStatsForActivityAndMetric(activity, metric_type, base::Time(),
                                        clock_->GetTime());
  }

  scoped_ptr<MetricVector> GetStatsForActivityAndMetric(
      MetricType metric_type) {
    return GetStatsForActivityAndMetric(kProcessChromeAggregate, metric_type,
                                        base::Time(), clock_->GetTime());
  }

  
  MetricVectorMap GetStatsForMetricByActivity(MetricType metric_type,
                                              const base::Time& start,
                                              const base::Time& end);

  MetricVectorMap GetStatsForMetricByActivity(MetricType metric_type) {
    return GetStatsForMetricByActivity(
        metric_type, base::Time(), clock_->GetTime());
  }

  
  
  std::vector<TimeRange> GetActiveIntervals(const base::Time& start,
                                            const base::Time& end);

  base::FilePath path() const { return path_; }

  void set_clock(scoped_ptr<Clock> clock) {
    clock_ = clock.Pass();
  }

 private:
  friend class DatabaseTestHelper;

  typedef std::map<std::string, std::string> RecentMap;
  typedef std::map<std::string, double> MaxValueMap;

  
  class SystemClock : public Clock {
   public:
    SystemClock() {}
    virtual ~SystemClock() {}
    virtual base::Time GetTime() OVERRIDE;
  };

  explicit Database(const base::FilePath& path);

  bool InitDBs();

  
  
  
  scoped_ptr<leveldb::DB> SafelyOpenDatabase(
      const leveldb::Options& options,
      const std::string& path,
      bool fix_if_damaged);

  bool Close();

  
  void LoadRecents();
  
  void LoadMaxValues();

  
  void UpdateActiveInterval();
  
  
  bool UpdateMaxValue(const std::string& activity,
                      MetricType metric,
                      const std::string& value);

  scoped_ptr<KeyBuilder> key_builder_;

  
  
  
  RecentMap recent_map_;

  MaxValueMap max_value_map_;

  
  base::FilePath path_;

  
  std::string start_time_key_;

  
  base::Time last_update_time_;

  scoped_ptr<Clock> clock_;

  scoped_ptr<leveldb::DB> recent_db_;

  scoped_ptr<leveldb::DB> max_value_db_;

  scoped_ptr<leveldb::DB> state_db_;

  scoped_ptr<leveldb::DB> active_interval_db_;

  scoped_ptr<leveldb::DB> metric_db_;

  scoped_ptr<leveldb::DB> event_db_;

  leveldb::ReadOptions read_options_;
  leveldb::WriteOptions write_options_;

  
  
  bool valid_;

  DISALLOW_COPY_AND_ASSIGN(Database);
};

}  

#endif  
