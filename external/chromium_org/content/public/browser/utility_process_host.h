// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_UTILITY_PROCESS_HOST_H_
#define CONTENT_PUBLIC_BROWSER_UTILITY_PROCESS_HOST_H_

#include "base/environment.h"
#include "base/process/launch.h"
#include "base/threading/thread.h"
#include "content/common/content_export.h"
#include "ipc/ipc_sender.h"

namespace base {
class FilePath;
class SequencedTaskRunner;
}

namespace content {
class UtilityProcessHostClient;
struct ChildProcessData;

typedef base::Thread* (*UtilityMainThreadFactoryFunction)(
    const std::string& id);

class UtilityProcessHost : public IPC::Sender,
                           public base::SupportsWeakPtr<UtilityProcessHost> {
 public:
  
  CONTENT_EXPORT static UtilityProcessHost* Create(
      UtilityProcessHostClient* client,
      base::SequencedTaskRunner* client_task_runner);

  virtual ~UtilityProcessHost() {}

  
  
  virtual bool StartBatchMode() = 0;

  
  virtual void EndBatchMode() = 0;

  
  
  virtual void SetExposedDir(const base::FilePath& dir) = 0;

  
  virtual void EnableMDns() = 0;

  
  virtual void DisableSandbox() = 0;

  
  virtual const ChildProcessData& GetData() = 0;

#if defined(OS_POSIX)
  virtual void SetEnv(const base::EnvironmentMap& env) = 0;
#endif

  CONTENT_EXPORT static void RegisterUtilityMainThreadFactory(
      UtilityMainThreadFactoryFunction create);
};

};  

#endif  
