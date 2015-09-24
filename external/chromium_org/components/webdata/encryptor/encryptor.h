// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_WEBDATA_ENCRYPTOR_ENCRYPTOR_H_
#define COMPONENTS_WEBDATA_ENCRYPTOR_ENCRYPTOR_H_

#include <string>

#include "base/strings/string16.h"

class Encryptor {
 public:
  
  
  
  static bool EncryptString16(const base::string16& plaintext,
                              std::string* ciphertext);

  
  
  
  
  static bool DecryptString16(const std::string& ciphertext,
                              base::string16* plaintext);

  
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
