// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROCESS_SINGLETON_STARTUP_LOCK_H_
#define CHROME_BROWSER_PROCESS_SINGLETON_STARTUP_LOCK_H_

#include <set>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/process_singleton.h"

class ProcessSingletonStartupLock : public base::NonThreadSafe {
 public:
  explicit ProcessSingletonStartupLock(
      const ProcessSingleton::NotificationCallback& original_callback);
  ~ProcessSingletonStartupLock();

  
  
  
  ProcessSingleton::NotificationCallback AsNotificationCallback();

  
  
  void Unlock();

  bool locked() { return locked_; }

 private:
  typedef std::pair<CommandLine::StringVector, base::FilePath>
      DelayedStartupMessage;

  bool NotificationCallbackImpl(const CommandLine& command_line,
                                const base::FilePath& current_directory);

  bool locked_;
  std::vector<DelayedStartupMessage> saved_startup_messages_;
  ProcessSingleton::NotificationCallback original_callback_;

  DISALLOW_COPY_AND_ASSIGN(ProcessSingletonStartupLock);
};

#endif  
