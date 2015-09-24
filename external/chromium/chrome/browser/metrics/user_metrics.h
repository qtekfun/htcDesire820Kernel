// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_METRICS_USER_METRICS_H_
#define CHROME_BROWSER_METRICS_USER_METRICS_H_
#pragma once

#include <string>

class Profile;



struct UserMetricsAction {
  const char* str_;
  explicit UserMetricsAction(const char* str) : str_(str) {}
};


class UserMetrics {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static void RecordAction(const UserMetricsAction& action, Profile* profile);

  
  
  
  
  
  static void RecordComputedAction(const std::string& action,
                                   Profile* profile);

  static void RecordAction(const UserMetricsAction& action);
  static void RecordComputedAction(const std::string& action);

 private:
  static void Record(const char *action, Profile *profile);
  static void Record(const char *action);
  static void CallRecordOnUI(const std::string& action);
};

#endif  
