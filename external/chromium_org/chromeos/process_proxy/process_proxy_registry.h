// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_PROCESS_PROXY_PROCESS_PROXY_REGISTRY_H_
#define CHROMEOS_PROCESS_PROXY_PROCESS_PROXY_REGISTRY_H_

#include <map>

#include "base/callback.h"
#include "base/lazy_instance.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/threading/non_thread_safe.h"
#include "base/threading/thread.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/process_proxy/process_proxy.h"

namespace chromeos {

typedef base::Callback<void(pid_t, const std::string&, const std::string&)>
      ProcessOutputCallbackWithPid;

class CHROMEOS_EXPORT ProcessProxyRegistry : public base::NonThreadSafe {
 public:
  
  struct ProcessProxyInfo {
    scoped_refptr<ProcessProxy> proxy;
    scoped_ptr<base::Thread> watcher_thread;
    ProcessOutputCallbackWithPid callback;
    pid_t process_id;

    ProcessProxyInfo();
    
    ProcessProxyInfo(const ProcessProxyInfo& other);
    ~ProcessProxyInfo();
  };

  static ProcessProxyRegistry* Get();

  
  bool OpenProcess(const std::string& command, pid_t* pid,
                   const ProcessOutputCallbackWithPid& callback);
  
  bool SendInput(pid_t pid, const std::string& data);
  
  bool CloseProcess(pid_t pid);
  
  bool OnTerminalResize(pid_t pid, int width, int height);

  
  void SetOutputCallback(const ProcessOutputCallback& callback);

 private:
  friend struct ::base::DefaultLazyInstanceTraits<ProcessProxyRegistry>;

  ProcessProxyRegistry();
  ~ProcessProxyRegistry();

  
  void OnProcessOutput(pid_t pid,
                       ProcessOutputType type,
                       const std::string& data);

  
  std::map<pid_t, ProcessProxyInfo> proxy_map_;

  DISALLOW_COPY_AND_ASSIGN(ProcessProxyRegistry);
};

}  

#endif  
