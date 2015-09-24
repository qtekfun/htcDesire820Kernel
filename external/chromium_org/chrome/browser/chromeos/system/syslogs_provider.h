// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SYSTEM_SYSLOGS_PROVIDER_H_
#define CHROME_BROWSER_CHROMEOS_SYSTEM_SYSLOGS_PROVIDER_H_

#include <map>
#include <string>

#include "base/callback.h"
#include "chrome/common/cancelable_task_tracker.h"

namespace chromeos {
namespace system {

extern const size_t kFeedbackMaxLength;

extern const size_t kFeedbackMaxLineCount;

typedef std::map<std::string, std::string> LogDictionaryType;

class SyslogsProvider {
 public:
  static SyslogsProvider* GetInstance();

  
  typedef base::Callback<void(LogDictionaryType*,
                              std::string*)> ReadCompleteCallback;

  
  enum SyslogsContext {
    SYSLOGS_FEEDBACK,
    SYSLOGS_SYSINFO,
    SYSLOGS_NETWORK,
    SYSLOGS_DEFAULT
  };

  
  
  
  
  
  virtual CancelableTaskTracker::TaskId RequestSyslogs(
      bool compress_logs,
      SyslogsContext context,
      const ReadCompleteCallback& callback,
      CancelableTaskTracker* tracker) = 0;

 protected:
  virtual ~SyslogsProvider() {}
};

}  
}  

#endif  
