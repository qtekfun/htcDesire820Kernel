// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_SIMPLE_RESOURCE_LOADER_H_
#define CHROME_FRAME_SIMPLE_RESOURCE_LOADER_H_

#include <windows.h>
#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/singleton.h"

namespace ui {
class DataPack;
}  

class SimpleResourceLoader {
 public:

  static SimpleResourceLoader* GetInstance();

  
  static std::wstring GetLanguage();

  
  
  static std::wstring Get(int message_id);

  
  
  HMODULE GetResourceModuleHandle();

  
  
  static void GetPreferredLanguages(std::vector<std::wstring>* language_tags);

  
  static void DetermineLocalesDirectory(base::FilePath* locales_path);

  
  static bool IsValidLanguageTag(const std::wstring& language_tag);

 private:
  SimpleResourceLoader();
  ~SimpleResourceLoader();

  
  
  
  
  
  
  
  static bool LoadLocalePack(const std::vector<std::wstring>& language_tags,
                             const base::FilePath& locales_path,
                             HMODULE* dll_handle,
                             ui::DataPack** data_pack,
                             std::wstring* language);

  
  
  std::wstring GetLocalizedResource(int message_id);

  friend struct DefaultSingletonTraits<SimpleResourceLoader>;

  FRIEND_TEST_ALL_PREFIXES(SimpleResourceLoaderTest, LoadLocaleDll);

  std::wstring language_;
  ui::DataPack* data_pack_;

  HINSTANCE locale_dll_handle_;
};

#endif  
