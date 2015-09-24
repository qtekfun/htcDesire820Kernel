// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SETTINGS_OWNER_KEY_UTIL_H_
#define CHROME_BROWSER_CHROMEOS_SETTINGS_OWNER_KEY_UTIL_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"

namespace base {
class FilePath;
}

namespace crypto {
class RSAPrivateKey;
}

namespace chromeos {

class OwnerKeyUtilTest;

class OwnerKeyUtil : public base::RefCountedThreadSafe<OwnerKeyUtil> {
 public:
  
  static OwnerKeyUtil* Create();

  
  
  virtual bool ImportPublicKey(std::vector<uint8>* output) = 0;

  
  
  
  virtual crypto::RSAPrivateKey* FindPrivateKey(
      const std::vector<uint8>& key) = 0;

  
  virtual bool IsPublicKeyPresent() = 0;

 protected:
  OwnerKeyUtil();
  virtual ~OwnerKeyUtil();

 private:
  friend class base::RefCountedThreadSafe<OwnerKeyUtil>;

  FRIEND_TEST_ALL_PREFIXES(OwnerKeyUtilTest, ExportImportPublicKey);
};

class OwnerKeyUtilImpl : public OwnerKeyUtil {
 public:
  explicit OwnerKeyUtilImpl(const base::FilePath& public_key_file);

  
  virtual bool ImportPublicKey(std::vector<uint8>* output) OVERRIDE;
  virtual crypto::RSAPrivateKey* FindPrivateKey(
      const std::vector<uint8>& key) OVERRIDE;
  virtual bool IsPublicKeyPresent() OVERRIDE;

 protected:
  virtual ~OwnerKeyUtilImpl();

 private:
  
  base::FilePath key_file_;

  DISALLOW_COPY_AND_ASSIGN(OwnerKeyUtilImpl);
};

}  

#endif  
