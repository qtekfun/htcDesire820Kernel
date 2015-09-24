// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ASH_MULTI_USER_MULTI_USER_UTIL_H_
#define CHROME_BROWSER_UI_ASH_MULTI_USER_MULTI_USER_UTIL_H_

#include <string>

class Profile;

namespace aura {
class Window;
}

namespace multi_user_util {

std::string GetUserIDFromProfile(Profile* profile);

std::string GetUserIDFromEmail(const std::string& email);

Profile* GetProfileFromUserID(const std::string& user_id);

Profile* GetProfileFromWindow(aura::Window* window);

bool IsProfileFromActiveUser(Profile* profile);

}  

#endif  
