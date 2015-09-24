// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREEN_OBSERVER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREEN_OBSERVER_H_
#pragma once

#include <string>

namespace chromeos {

class ScreenObserver {
 public:
  
  
  
  
  
  enum ExitCodes {
    NETWORK_CONNECTED,
    NETWORK_OFFLINE,
    ACCOUNT_CREATE_BACK,
    ACCOUNT_CREATED,
    CONNECTION_FAILED,
    UPDATE_INSTALLED,
    UPDATE_NOUPDATE,
    UPDATE_ERROR_CHECKING_FOR_UPDATE,
    UPDATE_ERROR_UPDATING,
    USER_IMAGE_SELECTED,
    USER_IMAGE_SKIPPED,
    EULA_ACCEPTED,
    EULA_BACK,
    REGISTRATION_SUCCESS,
    REGISTRATION_SKIPPED,
    ENTERPRISE_ENROLLMENT_CANCELLED,
    ENTERPRISE_ENROLLMENT_COMPLETED,
    EXIT_CODES_COUNT  
  };

  
  virtual void OnExit(ExitCodes exit_code) = 0;

  
  
  virtual void OnSetUserNamePassword(const std::string& username,
                                     const std::string& password) = 0;

  
  virtual void set_usage_statistics_reporting(bool val) = 0;
  virtual bool usage_statistics_reporting() const = 0;

 protected:
  virtual ~ScreenObserver() {}
};

}  

#endif  
