// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_ENCRYPTOR_H__
#define CHROME_BROWSER_PASSWORD_MANAGER_ENCRYPTOR_H__
#pragma once

#include <string>

#include "base/string16.h"

class Encryptor {
 public:
  
  
  
  static bool EncryptString16(const string16& plaintext,
                              std::string* ciphertext);

  
  
  
  
  static bool DecryptString16(const std::string& ciphertext,
                              string16* plaintext);

  
  static bool EncryptString(const std::string& plaintext,
                            std::string* ciphertext);

  
  
  
  
  static bool DecryptString(const std::string& ciphertext,
                            std::string* plaintext);

#if defined(OS_MACOSX)
  
  
  static void UseMockKeychain(bool use_mock);
#endif

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(Encryptor);
};

#endif  
