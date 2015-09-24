// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_WEBDATA_ENCRYPTOR_IE7_PASSWORD_H_
#define COMPONENTS_WEBDATA_ENCRYPTOR_IE7_PASSWORD_H_

#include <windows.h>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/time/time.h"

struct IE7PasswordInfo {
  
  std::wstring url_hash;

  
  
  std::vector<unsigned char> encrypted_data;

  
  base::Time date_created;
};

namespace ie7_password {

struct DecryptedCredentials {
  std::wstring username;
  std::wstring password;
};

bool GetUserPassFromData(const std::vector<unsigned char>& data,
                         std::vector<DecryptedCredentials>* credentials);

bool DecryptPasswords(const std::wstring& url,
                      const std::vector<unsigned char>& data,
                      std::vector<DecryptedCredentials>* credentials);

std::wstring GetUrlHash(const std::wstring& url);

}  

#endif  
