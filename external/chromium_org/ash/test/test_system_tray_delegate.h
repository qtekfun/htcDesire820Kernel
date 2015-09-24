// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_TEST_TEST_SYSTEM_TRAY_DELEGATE_H_
#define ASH_TEST_TEST_SYSTEM_TRAY_DELEGATE_H_

#include "ash/system/tray/default_system_tray_delegate.h"
#include "base/time/time.h"

namespace ash {
namespace test {

class TestSystemTrayDelegate : public DefaultSystemTrayDelegate {
 public:
  TestSystemTrayDelegate();
  virtual ~TestSystemTrayDelegate();

  
  
  
  
  
  static void SetInitialLoginStatus(user::LoginStatus login_status);

  
  
  
  
  
  void SetLoginStatus(user::LoginStatus login_status);

  void set_should_show_display_notification(bool should_show) {
    should_show_display_notification_ = should_show;
  }

  
  
  void SetSessionLengthLimitForTest(const base::TimeDelta& new_limit);

  
  void ClearSessionLengthLimit();

  
  virtual user::LoginStatus GetUserLoginStatus() const OVERRIDE;
  virtual bool ShouldShowDisplayNotification() OVERRIDE;
  virtual bool GetSessionStartTime(
      base::TimeTicks* session_start_time) OVERRIDE;
  virtual bool GetSessionLengthLimit(
      base::TimeDelta* session_length_limit) OVERRIDE;
  virtual void ShutDown() OVERRIDE;
  virtual void SignOut() OVERRIDE;

 private:
  bool should_show_display_notification_;
  user::LoginStatus login_status_;
  base::TimeDelta session_length_limit_;
  bool session_length_limit_set_;

  DISALLOW_COPY_AND_ASSIGN(TestSystemTrayDelegate);
};

}  
}  

#endif  
