// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_DEFAULT_PREF_STORE_H_
#define CHROME_BROWSER_PREFS_DEFAULT_PREF_STORE_H_
#pragma once

#include <string>

#include "base/values.h"
#include "chrome/browser/prefs/value_map_pref_store.h"

class DefaultPrefStore : public ValueMapPrefStore {
 public:
  DefaultPrefStore();
  virtual ~DefaultPrefStore();

  
  void SetDefaultValue(const std::string& key, Value* value);

  
  
  Value::ValueType GetType(const std::string& key) const;

 private:
  DISALLOW_COPY_AND_ASSIGN(DefaultPrefStore);
};

#endif  
