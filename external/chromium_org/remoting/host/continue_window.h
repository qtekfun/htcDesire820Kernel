// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_CONTINUE_WINDOW_H_
#define REMOTING_HOST_CONTINUE_WINDOW_H_

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "base/timer/timer.h"
#include "remoting/host/host_window.h"

namespace remoting {

class ContinueWindow : public HostWindow {
 public:
  virtual ~ContinueWindow();

  
  virtual void Start(
      const base::WeakPtr<ClientSessionControl>& client_session_control)
      OVERRIDE;

  
  void ContinueSession();

  
  void DisconnectSession();

 protected:
  ContinueWindow();

  
  virtual void ShowUi() = 0;
  virtual void HideUi() = 0;

 private:
  
  
  void OnSessionExpired();

  
  base::WeakPtr<ClientSessionControl> client_session_control_;

  
  base::OneShotTimer<ContinueWindow> disconnect_timer_;

  
  base::OneShotTimer<ContinueWindow> session_expired_timer_;

  DISALLOW_COPY_AND_ASSIGN(ContinueWindow);
};

}  

#endif  
