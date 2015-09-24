// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_VISIT_FILTER_H_
#define CHROME_BROWSER_HISTORY_VISIT_FILTER_H_

#include <vector>

#include "base/gtest_prod_util.h"
#include "base/time/time.h"

namespace history {

class VisitRow;

class VisitFilter {
 public:
  VisitFilter();
  virtual ~VisitFilter();

  
  
  
  
  
  typedef std::vector<std::pair<base::Time, base::Time> > TimeVector;

  
  const TimeVector& times() const {
    return times_;
  }

  
  
  void set_max_results(size_t max_results) {
    max_results_ = max_results;
    if (times_.size() > max_results_)
      times_.resize(max_results_);
  }

  
  
  void SetFilterTime(const base::Time& filter_time);

  
  
  
  
  
  
  
  void SetFilterWidth(const base::TimeDelta& filter_width);

  
  

  
  
  
  void SetDayOfTheWeekFilter(int day);

  
  
  
  void SetDayTypeFilter(bool workday);

  
  enum SortingOrder {
    ORDER_BY_RECENCY,  
    ORDER_BY_VISIT_COUNT,  
    ORDER_BY_DURATION_SPENT,  
                              
    ORDER_BY_TIME_GAUSSIAN,  
                             
                             
                             
    ORDER_BY_TIME_LINEAR,  
                           
                           
                           
  };

  double GetVisitScore(const VisitRow& visit) const;

  void set_sorting_order(SortingOrder order) {
    sorting_order_ = order;
    UpdateTimeVector();
  }

  SortingOrder sorting_order() const {
    return sorting_order_;
  }

  
  void ClearFilters();

 private:
  FRIEND_TEST_ALL_PREFIXES(VisitFilterTest, CheckFilters);
  FRIEND_TEST_ALL_PREFIXES(VisitFilterTest, GetTimesInRange);
  FRIEND_TEST_ALL_PREFIXES(VisitFilterTest, GetTimesOnTheDayOfTheWeek);
  FRIEND_TEST_ALL_PREFIXES(VisitFilterTest, GetTimesOnTheSameDayType);
  FRIEND_TEST_ALL_PREFIXES(VisitFilterTest, UniteTimeVectors);
  FRIEND_TEST_ALL_PREFIXES(VisitFilterTest, IntersectTimeVectors);

  
  bool UpdateTimeVector();

  
  static void GetTimesInRange(base::Time begin_time_of_the_day,
                              base::Time end_time_of_the_day,
                              size_t max_results,
                              TimeVector* times);

  
  
  
  static void GetTimesOnTheDayOfTheWeek(int day,
                                        base::Time week,
                                        size_t max_results,
                                        TimeVector* times);

  
  static void GetTimesOnTheSameDayType(bool workday,
                                       base::Time week,
                                       size_t max_results,
                                       TimeVector* times);

  
  
  static bool UniteTimeVectors(const TimeVector& vector1,
                               const TimeVector& vector2,
                               TimeVector* result);

  
  
  
  static bool IntersectTimeVectors(const TimeVector& vector1,
                                   const TimeVector& vector2,
                                   TimeVector* result);

  
  
  static base::TimeDelta GetTimeOfDayDifference(base::Time t1, base::Time t2);

  base::Time filter_time_;
  base::TimeDelta filter_width_;
  enum {
    DAY_UNDEFINED = -1,
    WORKDAY = 7,
    HOLIDAY = 8,
  };
  int day_;
  TimeVector times_;
  size_t max_results_;
  SortingOrder sorting_order_;
};

}  

#endif  
