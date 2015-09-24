// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTERNAL_METRICS_H_
#define CHROME_BROWSER_CHROMEOS_EXTERNAL_METRICS_H_
#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/hash_tables.h"
#include "base/task.h"

namespace chromeos {

class ExternalMetrics : public base::RefCountedThreadSafe<ExternalMetrics> {
  FRIEND_TEST_ALL_PREFIXES(ExternalMetricsTest, ParseExternalMetricsFile);
  friend class base::RefCountedThreadSafe<ExternalMetrics>;

 public:
  ExternalMetrics();

  
  
  
  void Start();

 private:
  
  typedef void (*RecordFunctionType)();

  typedef void (*RecorderType)(const char*, const char*);  

  
  static const int kMetricsMessageMaxLength = 1024;  

  ~ExternalMetrics() {}

  
  void RecordActionUI(std::string action_string);

  
  void RecordAction(const char* action_name);

  
  
  void RecordCrashUI(const std::string& crash_kind);

  
  void RecordCrash(const std::string& crash_kind);

  
  
  void RecordHistogram(const char* histogram_data);

  
  
  void RecordLinearHistogram(const char* histogram_data);

  
  
  void CollectEvents();

  
  void CollectEventsAndReschedule();

  
  void ScheduleCollector();

  
  base::hash_map<std::string, RecordFunctionType> action_recorders_;

  
  base::hash_set<std::string> valid_user_actions_;

  
  RecorderType test_recorder_;
  FilePath test_path_;
  DISALLOW_COPY_AND_ASSIGN(ExternalMetrics);
};

}  

#endif  
