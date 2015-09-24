// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_PERFORMANCE_MONITOR_PERFORMANCE_MONITOR_UI_UTIL_H_
#define CHROME_BROWSER_UI_WEBUI_PERFORMANCE_MONITOR_PERFORMANCE_MONITOR_UI_UTIL_H_

#include "chrome/browser/performance_monitor/database.h"
#include "chrome/browser/ui/webui/performance_monitor/performance_monitor_ui_constants.h"

namespace performance_monitor {

typedef std::vector<Database::MetricVector> VectorOfMetricVectors;

class PerformanceMonitorUtilTest;

class Aggregator {
 public:
  Aggregator();
  virtual ~Aggregator();

  
  
  
  
  
  scoped_ptr<VectorOfMetricVectors> AggregateMetrics(
      MetricType metric_type,
      const Database::MetricVector* metrics,
      const base::Time& start,
      const std::vector<TimeRange>& intervals,
      const base::TimeDelta& resolution);

 protected:
  friend class PerformanceMonitorUtilTest;

  
  
  
  
  
  
  virtual scoped_ptr<Database::MetricVector> AggregateInterval(
      MetricType metric_type,
      Database::MetricVector::const_iterator* metric,
      const Database::MetricVector::const_iterator& metric_end,
      const base::Time& time_start,
      const base::Time& time_end,
      const base::TimeDelta& resolution) = 0;
};

class NoAggregation : public Aggregator {
 private:
  virtual scoped_ptr<Database::MetricVector> AggregateInterval(
      MetricType metric_type,
      Database::MetricVector::const_iterator* metric,
      const Database::MetricVector::const_iterator& metric_end,
      const base::Time& time_start,
      const base::Time& time_end,
      const base::TimeDelta& resolution) OVERRIDE;
};

class MedianAggregation : public Aggregator {
 private:
  virtual scoped_ptr<Database::MetricVector> AggregateInterval(
      MetricType metric_type,
      Database::MetricVector::const_iterator* metric,
      const Database::MetricVector::const_iterator& metric_end,
      const base::Time& time_start,
      const base::Time& time_end,
      const base::TimeDelta& resolution) OVERRIDE;
};

class MeanAggregation : public Aggregator {
 private:
  virtual scoped_ptr<Database::MetricVector> AggregateInterval(
      MetricType metric_type,
      Database::MetricVector::const_iterator* metric,
      const Database::MetricVector::const_iterator& metric_end,
      const base::Time& time_start,
      const base::Time& time_end,
      const base::TimeDelta& resolution) OVERRIDE;
};

double GetConversionFactor(UnitDetails from, UnitDetails to);

scoped_ptr<std::vector<Database::MetricVector> > AggregateMetric(
    MetricType type,
    const Database::MetricVector* metrics,
    const base::Time& start,
    const std::vector<TimeRange>& intervals,
    const base::TimeDelta& resolution,
    AggregationMethod method);

}  

#endif  
