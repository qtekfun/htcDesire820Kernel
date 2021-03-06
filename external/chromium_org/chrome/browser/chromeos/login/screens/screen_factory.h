// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_SCREEN_FACTORY_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_SCREEN_FACTORY_H_

#include <string>

#include "base/basictypes.h"
#include "chrome/browser/chromeos/login/screens/base_screen.h"

namespace chromeos {

class OobeDisplay;
class ScreenObserver;

class ScreenFactory {
 public:
  static const char kEnrollmentScreenId[];
  static const char kErrorScreenId[];
  static const char kEulaScreenId[];
  static const char kKioskAutolaunchScreenId[];
  static const char kLocallyManagedUserCreationScreenId[];
  static const char kLoginScreenId[];
  static const char kNetworkScreenId[];
  static const char kResetScreenId[];
  static const char kTermsOfServiceScreenId[];
  static const char kUpdateScreenId[];
  static const char kUserImageScreenId[];
  static const char kWrongHWIDScreenId[];

  
  
  
  ScreenFactory(ScreenObserver* observer,
                OobeDisplay* oobe_display);
  virtual ~ScreenFactory();

  
  BaseScreen* CreateScreen(const std::string& id);

 private:
  BaseScreen* CreateScreenImpl(const std::string& id);

  
  
  ScreenObserver* observer_;

  
  OobeDisplay* oobe_display_;

  DISALLOW_COPY_AND_ASSIGN(ScreenFactory);
};

}  

#endif  
