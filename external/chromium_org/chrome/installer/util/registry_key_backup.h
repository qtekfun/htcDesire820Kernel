// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_REGISTRY_KEY_BACKUP_H_
#define CHROME_INSTALLER_UTIL_REGISTRY_KEY_BACKUP_H_

#include <windows.h>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

class RegistryKeyBackup {
 public:
  RegistryKeyBackup();
  ~RegistryKeyBackup();

  
  
  
  bool Initialize(HKEY root, const wchar_t* key_path);

  
  
  
  bool WriteTo(HKEY root, const wchar_t* key_path) const;

  void swap(RegistryKeyBackup& other) {
    key_data_.swap(other.key_data_);
  }

 private:
  class KeyData;

  
  scoped_ptr<KeyData> key_data_;

  DISALLOW_COPY_AND_ASSIGN(RegistryKeyBackup);
};

#endif  
