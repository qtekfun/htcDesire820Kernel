// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_REGISTRY_TEST_DATA_H_
#define CHROME_INSTALLER_UTIL_REGISTRY_TEST_DATA_H_

#include <windows.h>

#include <string>

#include "base/basictypes.h"

class RegistryTestData {
 public:
  RegistryTestData();
  
  ~RegistryTestData();

  
  
  
  
  
  bool Initialize(HKEY root_key, const wchar_t* base_path);

  
  void Reset();

  
  
  void ExpectMatchesNonEmptyKey(HKEY root_key, const wchar_t* path);

  HKEY root_key() const { return root_key_; }
  const std::wstring& base_path() const { return base_path_; }
  const std::wstring& empty_key_path() const { return empty_key_path_; }
  const std::wstring& non_empty_key_path() const { return non_empty_key_path_; }

  
  
  static void ExpectEmptyKey(HKEY root_key, const wchar_t* path);

 private:
  static bool DeleteKey(HKEY root_key, const wchar_t* path);

  HKEY root_key_;
  std::wstring base_path_;
  std::wstring empty_key_path_;
  std::wstring non_empty_key_path_;

  DISALLOW_COPY_AND_ASSIGN(RegistryTestData);
};

#endif  
