// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_SYSTEM_STORAGE_STORAGE_API_TEST_UTIL_H_
#define CHROME_BROWSER_EXTENSIONS_API_SYSTEM_STORAGE_STORAGE_API_TEST_UTIL_H_

#include <vector>

#include "chrome/browser/extensions/api/system_storage/storage_info_provider.h"
#include "chrome/browser/storage_monitor/storage_info.h"

namespace extensions {
namespace test {

struct TestStorageUnitInfo {
  const char* device_id;
  const char* name;
  
  double capacity;
  
  double available_capacity;
};

extern const struct TestStorageUnitInfo kRemovableStorageData;

StorageInfo BuildStorageInfoFromTestStorageUnitInfo(
    const TestStorageUnitInfo& unit);

}  
}  

#endif  
