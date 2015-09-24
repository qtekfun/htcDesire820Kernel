// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_STARTUP_TASK_RUNNER_SERVICE_H_
#define CHROME_BROWSER_PROFILES_STARTUP_TASK_RUNNER_SERVICE_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/threading/non_thread_safe.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

class Profile;

namespace base {
class DeferredSequencedTaskRunner;
}  

class StartupTaskRunnerService : public base::NonThreadSafe,
                                 public BrowserContextKeyedService {
 public:
  explicit StartupTaskRunnerService(Profile* profile);
  virtual ~StartupTaskRunnerService();

  
  
  
  
  
  
  
  scoped_refptr<base::DeferredSequencedTaskRunner> GetBookmarkTaskRunner();

  
  void StartDeferredTaskRunners();

 private:
  Profile* profile_;
  scoped_refptr<base::DeferredSequencedTaskRunner> bookmark_task_runner_;

  DISALLOW_COPY_AND_ASSIGN(StartupTaskRunnerService);
};

#endif  
