// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_TEST_REG_UTIL_H_
#define BASE_TEST_TEST_REG_UTIL_H_


#include "base/basictypes.h"
#include "base/memory/scoped_vector.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "base/win/registry.h"

namespace registry_util {

class RegistryOverrideManager {
 public:
  RegistryOverrideManager();
  ~RegistryOverrideManager();

  
  
  
  
  void OverrideRegistry(HKEY override, const string16& override_name);

 private:
  friend class RegistryOverrideManagerTest;

  
  class ScopedRegistryKeyOverride {
   public:
    ScopedRegistryKeyOverride(HKEY override, const string16& key_path);
    ~ScopedRegistryKeyOverride();

   private:
    HKEY override_;
    base::win::RegKey temp_key_;

    DISALLOW_COPY_AND_ASSIGN(ScopedRegistryKeyOverride);
  };

  
  RegistryOverrideManager(const base::Time& timestamp,
                          const string16& test_key_root);

  base::Time timestamp_;
  string16 guid_;

  string16 test_key_root_;
  ScopedVector<ScopedRegistryKeyOverride> overrides_;

  DISALLOW_COPY_AND_ASSIGN(RegistryOverrideManager);
};

string16 GenerateTempKeyPath();

}  

#endif  
