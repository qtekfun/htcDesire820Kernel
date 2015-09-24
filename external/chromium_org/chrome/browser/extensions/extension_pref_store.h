// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_PREF_STORE_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_PREF_STORE_H_

#include <string>

#include "base/prefs/value_map_pref_store.h"
#include "chrome/browser/extensions/extension_pref_value_map.h"

class ExtensionPrefStore : public ValueMapPrefStore,
                           public ExtensionPrefValueMap::Observer {
 public:
  
  ExtensionPrefStore(ExtensionPrefValueMap* extension_pref_value_map,
                     bool incognito_pref_store);

  
  virtual void OnInitializationCompleted() OVERRIDE;
  virtual void OnPrefValueChanged(const std::string& key) OVERRIDE;
  virtual void OnExtensionPrefValueMapDestruction() OVERRIDE;

 protected:
  virtual ~ExtensionPrefStore();

 private:
  ExtensionPrefValueMap* extension_pref_value_map_;  
  bool incognito_pref_store_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionPrefStore);
};

#endif  
