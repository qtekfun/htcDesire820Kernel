// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_NETWORK_SCREEN_ACTOR_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_NETWORK_SCREEN_ACTOR_H_

#include "base/strings/string16.h"

namespace chromeos {

class NetworkScreenActor {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}
    virtual void OnActorDestroyed(NetworkScreenActor* actor) = 0;
    virtual void OnContinuePressed() = 0;
  };

  virtual ~NetworkScreenActor() {}

  
  virtual void SetDelegate(Delegate* screen) = 0;

  
  virtual void PrepareToShow() = 0;

  
  virtual void Show() = 0;

  
  virtual void Hide() = 0;

  
  virtual void ShowError(const base::string16& message) = 0;

  
  virtual void ClearErrors() = 0;

  
  virtual void ShowConnectingStatus(
      bool connecting,
      const base::string16& network_id) = 0;

  
  virtual void EnableContinue(bool enabled) = 0;
};

}  

#endif  
