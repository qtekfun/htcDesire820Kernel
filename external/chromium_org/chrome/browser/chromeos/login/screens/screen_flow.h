// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_SCREEN_FLOW_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_SCREEN_FLOW_H_

#include <string>

#include "base/basictypes.h"

namespace chromeos {

class ScreenContext;
class ScreenManager;

class ScreenFlow {
 public:
  ScreenFlow();
  virtual ~ScreenFlow();

  
  
  virtual void Start() = 0;

  
  
  
  
  virtual void OnScreenFinished(const std::string& screen_id,
                                const std::string& outcome,
                                ScreenContext* context) = 0;
 protected:
  ScreenManager* screen_manager() { return manager_; }

 private:
  friend class ScreenManager;

  void set_screen_manager(ScreenManager* manager) {
    manager_ = manager;
  }

  
  ScreenManager* manager_;

  DISALLOW_COPY_AND_ASSIGN(ScreenFlow);
};

}  

#endif  
