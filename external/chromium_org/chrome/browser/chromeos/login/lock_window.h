// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_LOCK_WINDOW_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_LOCK_WINDOW_H_

#include "base/basictypes.h"

namespace views {
class Widget;
}

namespace chromeos {

class LockWindow {
 public:
  
  
  class Observer {
   public:
    
    
    virtual void OnLockWindowReady() = 0;
  };

  LockWindow();

  
  
  virtual void Grab() = 0;

  
  virtual views::Widget* GetWidget() = 0;

  
  void set_observer(Observer* observer) {
    observer_ = observer;
  }

  
  static LockWindow* Create();

 protected:
  
  
  Observer* observer_;

  DISALLOW_COPY_AND_ASSIGN(LockWindow);
};

}  

#endif  
