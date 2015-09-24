// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_WEB_REQUEST_WEB_REQUEST_TIME_TRACKER_H_
#define CHROME_BROWSER_EXTENSIONS_API_WEB_REQUEST_WEB_REQUEST_TIME_TRACKER_H_

#include <map>
#include <queue>
#include <set>
#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "url/gurl.h"

namespace base {
class Time;
}

class ExtensionWebRequestTimeTrackerDelegate {
 public:
  virtual ~ExtensionWebRequestTimeTrackerDelegate() {}

  
  
  
  
  virtual void NotifyExcessiveDelays(
      void* profile,
      size_t num_delayed_messages,
      size_t total_num_messages,
      const std::set<std::string>& extension_ids) = 0;
  virtual void NotifyModerateDelays(
      void* profile,
      size_t num_delayed_messages,
      size_t total_num_messages,
      const std::set<std::string>& extension_ids) = 0;
};

class ExtensionWebRequestTimeTracker {
 public:
  ExtensionWebRequestTimeTracker();
  ~ExtensionWebRequestTimeTracker();

  
  void LogRequestStartTime(int64 request_id, const base::Time& start_time,
                           const GURL& url, void* profile);

  
  void LogRequestEndTime(int64 request_id, const base::Time& end_time);

  
  
  void IncrementExtensionBlockTime(
      const std::string& extension_id,
      int64 request_id,
      const base::TimeDelta& block_time);

  
  
  void IncrementTotalBlockTime(
      int64 request_id,
      const base::TimeDelta& block_time);

  
  void SetRequestCanceled(int64 request_id);

  
  void SetRequestRedirected(int64 request_id);

  
  void SetDelegate(ExtensionWebRequestTimeTrackerDelegate* delegate);

 private:
  
  struct RequestTimeLog {
    GURL url;  
    void* profile;  
    bool completed;
    base::Time request_start_time;
    base::TimeDelta request_duration;
    base::TimeDelta block_duration;
    std::map<std::string, base::TimeDelta> extension_block_durations;
    RequestTimeLog();
    ~RequestTimeLog();
  };

  
  
  void Analyze(int64 request_id);

  
  std::set<std::string> GetExtensionIds(const RequestTimeLog& log) const;

  
  std::map<int64, RequestTimeLog> request_time_logs_;

  
  
  std::queue<int64> request_ids_;

  
  
  std::set<int64> excessive_delays_;
  std::set<int64> moderate_delays_;

  
  scoped_ptr<ExtensionWebRequestTimeTrackerDelegate> delegate_;

  FRIEND_TEST_ALL_PREFIXES(ExtensionWebRequestTimeTrackerTest, Basic);
  FRIEND_TEST_ALL_PREFIXES(ExtensionWebRequestTimeTrackerTest,
                           IgnoreFastRequests);
  FRIEND_TEST_ALL_PREFIXES(ExtensionWebRequestTimeTrackerTest,
                           CancelOrRedirect);
  FRIEND_TEST_ALL_PREFIXES(ExtensionWebRequestTimeTrackerTest, Delays);

  DISALLOW_COPY_AND_ASSIGN(ExtensionWebRequestTimeTracker);
};

#endif  
