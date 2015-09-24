// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_STORAGE_LEVELDB_SETTINGS_STORAGE_FACTORY_H_
#define CHROME_BROWSER_EXTENSIONS_API_STORAGE_LEVELDB_SETTINGS_STORAGE_FACTORY_H_

#include "chrome/browser/extensions/api/storage/settings_storage_factory.h"

namespace extensions {

class LeveldbSettingsStorageFactory : public SettingsStorageFactory {
 public:
  virtual ValueStore* Create(const base::FilePath& base_path,
                             const std::string& extension_id) OVERRIDE;

 private:
  
  virtual ~LeveldbSettingsStorageFactory() {}
};

}  

#endif  
