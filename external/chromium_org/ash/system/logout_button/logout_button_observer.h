// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_LOGOUT_BUTTON_LOGOUT_BUTTON_OBSERVER_H_
#define ASH_SYSTEM_LOGOUT_BUTTON_LOGOUT_BUTTON_OBSERVER_H_

namespace ash {

class LogoutButtonObserver {
 public:
  virtual ~LogoutButtonObserver() {}

  
  virtual void OnShowLogoutButtonInTrayChanged(bool show) = 0;
};

}  

#endif  
