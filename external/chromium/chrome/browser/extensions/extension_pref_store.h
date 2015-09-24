// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_PREF_STORE_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_PREF_STORE_H_
#pragma once

#include <string>

#include "base/time.h"
#include "chrome/browser/extensions/extension_pref_value_map.h"
#include "chrome/browser/prefs/value_map_pref_store.h"

class ExtensionPrefStore : public ValueMapPrefStore,
                           public ExtensionPrefValueMap::Observer {
 public:
  
  explicit ExtensionPrefStore(ExtensionPrefValueMap* extension_pref_value_map,
                              bool incognito_pref_store);
  virtual ~ExtensionPrefStore();

 private:
  
  virtual void OnInitializationCompleted();
  virtual void OnPrefValueChanged(const std::string& key);
  virtual void OnExtensionPrefValueMapDestruction();

  ExtensionPrefValueMap* extension_pref_value_map_;  
  bool incognito_pref_store_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionPrefStore);
};

#endif  
