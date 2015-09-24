// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_SIGNIN_UI_UTIL_H_
#define CHROME_BROWSER_SIGNIN_SIGNIN_UI_UTIL_H_

#include <vector>

#include "base/strings/string16.h"

class GlobalError;
class Profile;
class SigninManagerBase;

namespace signin_ui_util {

GlobalError* GetSignedInServiceError(Profile* profile);

std::vector<GlobalError*> GetSignedInServiceErrors(Profile* profile);

base::string16 GetSigninMenuLabel(Profile* profile);

void GetStatusLabelsForAuthError(Profile* profile,
                                 const SigninManagerBase& signin_manager,
                                 base::string16* status_label,
                                 base::string16* link_label);

}  

#endif  
