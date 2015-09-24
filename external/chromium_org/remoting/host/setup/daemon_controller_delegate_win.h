// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_SETUP_DAEMON_CONTROLLER_DELEGATE_WIN_H_
#define REMOTING_HOST_SETUP_DAEMON_CONTROLLER_DELEGATE_WIN_H_

#include "base/memory/scoped_ptr.h"
#include "base/timer/timer.h"
#include "base/win/scoped_comptr.h"
#include "remoting/host/chromoting_lib.h"
#include "remoting/host/setup/daemon_controller.h"

namespace remoting {

class DaemonInstallerWin;

class DaemonControllerDelegateWin : public DaemonController::Delegate {
 public:
  DaemonControllerDelegateWin();
  virtual ~DaemonControllerDelegateWin();

  
  virtual DaemonController::State GetState() OVERRIDE;
  virtual scoped_ptr<base::DictionaryValue> GetConfig() OVERRIDE;
  virtual void SetConfigAndStart(
      scoped_ptr<base::DictionaryValue> config,
      bool consent,
      const DaemonController::CompletionCallback& done) OVERRIDE;
  virtual void UpdateConfig(
      scoped_ptr<base::DictionaryValue> config,
      const DaemonController::CompletionCallback& done) OVERRIDE;
  virtual void Stop(const DaemonController::CompletionCallback& done) OVERRIDE;
  virtual void SetWindow(void* window_handle) OVERRIDE;
  virtual std::string GetVersion() OVERRIDE;
  virtual DaemonController::UsageStatsConsent GetUsageStatsConsent() OVERRIDE;

 private:
  
  HRESULT ActivateController();

  
  
  
  HRESULT ActivateElevatedController();

  
  void ReleaseController();

  
  
  void OnInstallationComplete(
      scoped_ptr<base::DictionaryValue> config,
      bool consent,
      const DaemonController::CompletionCallback& done,
      HRESULT hr);

  
  
  base::win::ScopedComPtr<IDaemonControl> control_;
  base::win::ScopedComPtr<IDaemonControl2> control2_;

  
  
  bool control_is_elevated_;

  
  scoped_ptr<base::OneShotTimer<DaemonControllerDelegateWin> > release_timer_;

  
  HWND window_handle_;

  scoped_ptr<DaemonInstallerWin> installer_;

  DISALLOW_COPY_AND_ASSIGN(DaemonControllerDelegateWin);
};

}  

#endif  
