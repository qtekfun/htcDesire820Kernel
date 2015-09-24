// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_PROFILE_AUTH_DATA_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_PROFILE_AUTH_DATA_H_

#include <string>
#include "base/callback.h"

class Profile;

namespace chromeos {

class ProfileAuthData {
 public:
  
  
  
  
  static void Transfer(Profile* from_profile,
                       Profile* to_profile,
                       bool transfer_cookies,
                       const base::Closure& completion_callback);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(ProfileAuthData);
};

}  

#endif  
