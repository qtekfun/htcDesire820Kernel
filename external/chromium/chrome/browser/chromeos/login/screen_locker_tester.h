// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREEN_LOCKER_TESTER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREEN_LOCKER_TESTER_H_
#pragma once

#include <string>

#include "base/basictypes.h"

namespace views {
class Button;
class Textfield;
class Widget;
}  

namespace chromeos {

class ScreenLocker;

namespace test {

class ScreenLockerTester {
 public:
  
  bool IsLocked();

  
  void InjectMockAuthenticator(const std::string& user,
                               const std::string& password);

  
  void SetPassword(const std::string& password);

  
  std::string GetPassword() const;

  
  void EnterPassword(const std::string& password);

  
  void EmulateWindowManagerReady();

  
  views::Widget* GetWidget() const;

  views::Widget* GetChildWidget() const;

 private:
  friend class chromeos::ScreenLocker;

  ScreenLockerTester() {}

  views::Textfield* GetPasswordField() const;

  DISALLOW_COPY_AND_ASSIGN(ScreenLockerTester);
};

}  

}  

#endif  
