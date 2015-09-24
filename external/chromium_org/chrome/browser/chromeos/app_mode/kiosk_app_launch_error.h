// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_APP_MODE_KIOSK_APP_LAUNCH_ERROR_H_
#define CHROME_BROWSER_CHROMEOS_APP_MODE_KIOSK_APP_LAUNCH_ERROR_H_

#include <string>

#include "base/basictypes.h"

namespace chromeos {

class KioskAppLaunchError {
 public:
  enum Error {
    NONE,                     
    HAS_PENDING_LAUNCH,       
    CRYPTOHOMED_NOT_RUNNING,  
    ALREADY_MOUNTED,          
    UNABLE_TO_MOUNT,          
    UNABLE_TO_REMOVE,         
    UNABLE_TO_INSTALL,        
    USER_CANCEL,              
    NOT_KIOSK_ENABLED,        
    UNABLE_TO_RETRIEVE_HASH,  
    POLICY_LOAD_FAILED,       
  };

  
  static std::string GetErrorMessage(Error error);

  
  
  static void Save(Error error);

  
  static Error Get();

  
  static void Clear();

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(KioskAppLaunchError);
};

}  

#endif  
