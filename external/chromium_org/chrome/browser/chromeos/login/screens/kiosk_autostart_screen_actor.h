// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_KIOSK_AUTOSTART_SCREEN_ACTOR_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_KIOSK_AUTOSTART_SCREEN_ACTOR_H_

#include <string>

namespace chromeos {

class KioskAutostartScreenActor {
 public:
  
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual void OnExit() = 0;

    
    
    virtual void OnActorDestroyed(KioskAutostartScreenActor* actor) = 0;
  };

  virtual ~KioskAutostartScreenActor() {}

  virtual void PrepareToShow() = 0;
  virtual void Show() = 0;
  virtual void Hide() = 0;
  virtual void SetDelegate(Delegate* delegate) = 0;
};

}  

#endif  
