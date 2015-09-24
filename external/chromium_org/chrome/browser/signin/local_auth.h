// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_LOCAL_AUTH_H_
#define CHROME_BROWSER_SIGNIN_LOCAL_AUTH_H_

#include <string>

class Profile;

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace chrome {

void RegisterLocalAuthPrefs(user_prefs::PrefRegistrySyncable* registry);

void SetLocalAuthCredentials(size_t profile_info_index,
                             const std::string& password);

void SetLocalAuthCredentials(const Profile* profile,
                             const std::string& password);

bool ValidateLocalAuthCredentials(size_t profile_info_index,
                                  const std::string& password);

bool ValidateLocalAuthCredentials(const Profile* profile,
                                  const std::string& password);

}  

#endif  
