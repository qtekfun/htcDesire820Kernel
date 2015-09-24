// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_TEST_EXTENSION_PREFS_H_
#define CHROME_BROWSER_EXTENSIONS_TEST_EXTENSION_PREFS_H_

#include <string>

#include "base/files/scoped_temp_dir.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "extensions/common/manifest.h"

class ExtensionPrefValueMap;
class PrefService;
class PrefServiceSyncable;

namespace base {
class DictionaryValue;
class SequencedTaskRunner;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace extensions {
class Extension;
class ExtensionPrefs;

class TestExtensionPrefs {
 public:
  explicit TestExtensionPrefs(base::SequencedTaskRunner* task_runner);
  virtual ~TestExtensionPrefs();

  ExtensionPrefs* prefs() { return prefs_.get(); }
  const ExtensionPrefs& const_prefs() const {
      return *prefs_.get();
  }
  PrefService* pref_service();
  const scoped_refptr<user_prefs::PrefRegistrySyncable>& pref_registry();
  void ResetPrefRegistry();
  const base::FilePath& temp_dir() const { return temp_dir_.path(); }
  const base::FilePath& extensions_dir() const { return extensions_dir_; }
  ExtensionPrefValueMap* extension_pref_value_map() {
    return extension_pref_value_map_.get();
  }

  
  
  void RecreateExtensionPrefs();

  
  
  scoped_refptr<Extension> AddExtension(std::string name);

  
  scoped_refptr<Extension> AddApp(std::string name);

  
  scoped_refptr<Extension> AddExtensionWithManifest(
      const base::DictionaryValue& manifest,
      Manifest::Location location);

  
  
  scoped_refptr<Extension> AddExtensionWithManifestAndFlags(
      const base::DictionaryValue& manifest,
      Manifest::Location location,
      int extra_flags);

  
  
  
  std::string AddExtensionAndReturnId(std::string name);

  PrefService* CreateIncognitoPrefService() const;

  
  
  void set_extensions_disabled(bool extensions_disabled);

 protected:
  base::ScopedTempDir temp_dir_;
  base::FilePath preferences_file_;
  base::FilePath extensions_dir_;
  scoped_refptr<user_prefs::PrefRegistrySyncable> pref_registry_;
  scoped_ptr<PrefServiceSyncable> pref_service_;
  scoped_ptr<ExtensionPrefs> prefs_;
  scoped_ptr<ExtensionPrefValueMap> extension_pref_value_map_;
  const scoped_refptr<base::SequencedTaskRunner> task_runner_;

 private:
  bool extensions_disabled_;
  DISALLOW_COPY_AND_ASSIGN(TestExtensionPrefs);
};

}  

#endif  
