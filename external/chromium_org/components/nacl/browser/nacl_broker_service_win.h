// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_NACL_BROWSER_NACL_BROKER_SERVICE_WIN_H_
#define COMPONENTS_NACL_BROWSER_NACL_BROKER_SERVICE_WIN_H_

#include <map>

#include "base/basictypes.h"
#include "base/memory/singleton.h"
#include "base/memory/weak_ptr.h"
#include "components/nacl/browser/nacl_broker_host_win.h"

namespace nacl {

class NaClProcessHost;

class NaClBrokerService {
 public:
  
  static NaClBrokerService* GetInstance();

  
  bool StartBroker();

  
  
  bool LaunchLoader(base::WeakPtr<NaClProcessHost> client,
                    const std::string& loader_channel_id);

  
  void OnLoaderLaunched(const std::string& channel_id,
                        base::ProcessHandle handle);

  
  void OnLoaderDied();

  bool LaunchDebugExceptionHandler(base::WeakPtr<NaClProcessHost> client,
                                   int32 pid,
                                   base::ProcessHandle process_handle,
                                   const std::string& startup_info);

  
  
  void OnDebugExceptionHandlerLaunched(int32 pid, bool success);

 private:
  typedef std::map<std::string, base::WeakPtr<NaClProcessHost> >
      PendingLaunchesMap;
  typedef std::map<int, base::WeakPtr<NaClProcessHost> >
      PendingDebugExceptionHandlersMap;

  friend struct DefaultSingletonTraits<NaClBrokerService>;

  NaClBrokerService();
  ~NaClBrokerService() {}

  NaClBrokerHost* GetBrokerHost();

  int loaders_running_;
  PendingLaunchesMap pending_launches_;
  PendingDebugExceptionHandlersMap pending_debuggers_;

  DISALLOW_COPY_AND_ASSIGN(NaClBrokerService);
};

}  

#endif  
