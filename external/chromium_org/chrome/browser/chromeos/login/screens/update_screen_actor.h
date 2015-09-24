// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_UPDATE_SCREEN_ACTOR_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_UPDATE_SCREEN_ACTOR_H_

#include "base/time/time.h"

namespace chromeos {

class UpdateScreenActor {
 public:
  
  enum ProgressMessage {
    PROGRESS_MESSAGE_UPDATE_AVAILABLE = 0,
    PROGRESS_MESSAGE_INSTALLING_UPDATE,
    PROGRESS_MESSAGE_VERIFYING,
    PROGRESS_MESSAGE_FINALIZING
  };

  class Delegate {
   public:
    virtual ~Delegate() {}
    
    virtual void CancelUpdate() = 0;
    virtual void OnActorDestroyed(UpdateScreenActor* actor) = 0;
    virtual void OnConnectToNetworkRequested(
        const std::string& service_path) = 0;
  };

  virtual ~UpdateScreenActor() {}

  
  virtual void SetDelegate(Delegate* screen) = 0;

  
  virtual void Show() = 0;

  
  virtual void Hide() = 0;

  virtual void PrepareToShow() = 0;

  
  virtual void ShowManualRebootInfo() = 0;

  
  virtual void SetProgress(int progress) = 0;

  
  virtual void ShowEstimatedTimeLeft(bool visible) = 0;

  
  virtual void SetEstimatedTimeLeft(const base::TimeDelta& time) = 0;

  
  virtual void ShowProgressMessage(bool visible) = 0;

  
  virtual void SetProgressMessage(ProgressMessage message) = 0;

  
  virtual void ShowCurtain(bool visible) = 0;
};

}  

#endif  
