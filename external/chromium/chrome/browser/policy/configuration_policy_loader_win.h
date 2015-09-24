// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_CONFIGURATION_POLICY_LOADER_WIN_H_
#define CHROME_BROWSER_POLICY_CONFIGURATION_POLICY_LOADER_WIN_H_
#pragma once

#include "base/synchronization/waitable_event.h"
#include "base/win/object_watcher.h"
#include "chrome/browser/policy/asynchronous_policy_loader.h"

namespace policy {

class ConfigurationPolicyLoaderWin
    : public AsynchronousPolicyLoader,
      public base::win::ObjectWatcher::Delegate {
 public:
  ConfigurationPolicyLoaderWin(
      AsynchronousPolicyProvider::Delegate* delegate,
      int reload_interval_minutes);
  virtual ~ConfigurationPolicyLoaderWin() {}

 protected:
  
  virtual void InitOnFileThread();
  virtual void StopOnFileThread();

 private:
  
  void SetupWatches();

  
  void Reload();

  
  virtual void OnObjectSignaled(HANDLE object);

  base::WaitableEvent user_policy_changed_event_;
  base::WaitableEvent machine_policy_changed_event_;
  base::win::ObjectWatcher user_policy_watcher_;
  base::win::ObjectWatcher machine_policy_watcher_;
  bool user_policy_watcher_failed_;
  bool machine_policy_watcher_failed_;

  DISALLOW_COPY_AND_ASSIGN(ConfigurationPolicyLoaderWin);
};

}  

#endif  
