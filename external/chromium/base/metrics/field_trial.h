// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.











#ifndef BASE_METRICS_FIELD_TRIAL_H_
#define BASE_METRICS_FIELD_TRIAL_H_
#pragma once

#include <map>
#include <string>

#include "base/base_api.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "base/time.h"

namespace base {

class FieldTrialList;

class BASE_API FieldTrial : public RefCounted<FieldTrial> {
 public:
  typedef int Probability;  

  
  
  static const int kNotFinalized;

  
  
  static const int kDefaultGroupNumber;

  
  
  
  
  
  
  
  FieldTrial(const std::string& name, Probability total_probability,
             const std::string& default_group_name, const int year,
             const int month, const int day_of_month);

  
  
  
  
  int AppendGroup(const std::string& name, Probability group_probability);

  
  std::string name() const { return name_; }

  
  
  
  
  int group();

  
  
  
  std::string group_name();

  
  std::string default_group_name() const { return default_group_name_; }

  
  
  static std::string MakeName(const std::string& name_prefix,
                              const std::string& trial_name);

  
  static void EnableBenchmarking();

 private:
  
  FRIEND_TEST(FieldTrialTest, Registration);
  FRIEND_TEST(FieldTrialTest, AbsoluteProbabilities);
  FRIEND_TEST(FieldTrialTest, RemainingProbability);
  FRIEND_TEST(FieldTrialTest, FiftyFiftyProbability);
  FRIEND_TEST(FieldTrialTest, MiddleProbabilities);
  FRIEND_TEST(FieldTrialTest, OneWinner);
  FRIEND_TEST(FieldTrialTest, DisableProbability);
  FRIEND_TEST(FieldTrialTest, Save);
  FRIEND_TEST(FieldTrialTest, DuplicateRestore);
  FRIEND_TEST(FieldTrialTest, MakeName);

  friend class base::FieldTrialList;

  friend class RefCounted<FieldTrial>;

  virtual ~FieldTrial();

  
  std::string group_name_internal() const { return group_name_; }

  
  static Time GetBuildTime();

  
  
  const std::string name_;

  
  
  const Probability divisor_;

  
  const std::string default_group_name_;

  
  
  
  const Probability random_;

  
  Probability accumulated_group_probability_;

  int next_group_number_;

  
  
  int group_;

  
  
  std::string group_name_;

  
  
  bool disable_field_trial_;

  
  
  static bool enable_benchmarking_;

  DISALLOW_COPY_AND_ASSIGN(FieldTrial);
};

class BASE_API FieldTrialList {
 public:
  
  
  
  static const char kPersistentStringSeparator;  

  
  FieldTrialList();
  
  ~FieldTrialList();

  
  
  static void Register(FieldTrial* trial);

  
  
  static FieldTrial* Find(const std::string& name);

  static int FindValue(const std::string& name);

  static std::string FindFullName(const std::string& name);

  
  
  
  
  
  static void StatesToString(std::string* output);

  
  
  
  
  
  
  static bool CreateTrialsInChildProcess(const std::string& prior_trials);

  
  
  
  
  
  static TimeTicks application_start_time() {
    if (global_)
      return global_->application_start_time_;
    
    return TimeTicks::Now();
  }

  
  static size_t GetFieldTrialCount();

 private:
  
  typedef std::map<std::string, FieldTrial*> RegistrationList;

  
  FieldTrial* PreLockedFind(const std::string& name);

  static FieldTrialList* global_;  

  
  
  
  static bool register_without_global_;

  
  
  
  TimeTicks application_start_time_;

  
  base::Lock lock_;
  RegistrationList registered_;

  DISALLOW_COPY_AND_ASSIGN(FieldTrialList);
};

}  

#endif  

