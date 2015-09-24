// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_IE7_PASSWORD_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_IE7_PASSWORD_H_
#pragma once

#include <windows.h>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/time.h"

struct IE7PasswordInfo {
  
  std::wstring url_hash;

  
  
  std::vector<unsigned char> encrypted_data;

  
  base::Time date_created;
};

namespace ie7_password {

bool GetUserPassFromData(const std::vector<unsigned char>& data,
                         std::wstring* username,
                         std::wstring* password);

bool DecryptPassword(const std::wstring& url,
                     const std::vector<unsigned char>& data,
                     std::wstring* username, std::wstring* password);

std::wstring GetUrlHash(const std::wstring& url);

}  

#endif  
