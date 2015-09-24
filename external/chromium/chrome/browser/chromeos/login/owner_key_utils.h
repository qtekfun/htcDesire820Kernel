// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_OWNER_KEY_UTILS_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_OWNER_KEY_UTILS_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/chromeos/cros/login_library.h"

class FilePath;

namespace crypto {
class RSAPrivateKey;
}

namespace chromeos {

class OwnerKeyUtilsTest;

class OwnerKeyUtils : public base::RefCounted<OwnerKeyUtils> {
 public:
  class Factory {
   public:
    virtual OwnerKeyUtils* CreateOwnerKeyUtils() = 0;
  };

  OwnerKeyUtils();

  
  
  
  
#if defined(UNIT_TEST)
  static void set_factory(Factory* factory) { factory_ = factory; }
#endif

  
  
  static OwnerKeyUtils* Create();

  
  
  virtual bool ImportPublicKey(const FilePath& key_file,
                               std::vector<uint8>* output) = 0;

  
  
  
  virtual bool Verify(const std::string& data,
                      const std::vector<uint8> signature,
                      const std::vector<uint8> public_key) = 0;

  
  
  virtual bool Sign(const std::string& data,
                    std::vector<uint8>* OUT_signature,
                    crypto::RSAPrivateKey* key) = 0;

  
  
  
  virtual crypto::RSAPrivateKey* FindPrivateKey(
      const std::vector<uint8>& key) = 0;

  virtual FilePath GetOwnerKeyFilePath() = 0;

 protected:
  virtual ~OwnerKeyUtils();

  
  
  
  virtual bool ExportPublicKeyToFile(crypto::RSAPrivateKey* pair,
                                     const FilePath& key_file) = 0;

 private:
  friend class base::RefCounted<OwnerKeyUtils>;
  static Factory* factory_;

  FRIEND_TEST_ALL_PREFIXES(OwnerKeyUtilsTest, ExportImportPublicKey);
};

}  

#endif  
