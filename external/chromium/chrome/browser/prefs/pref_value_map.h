// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_PREF_VALUE_MAP_H_
#define CHROME_BROWSER_PREFS_PREF_VALUE_MAP_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"

class Value;

class PrefValueMap {
 public:
  typedef std::map<std::string, Value*>::iterator iterator;
  typedef std::map<std::string, Value*>::const_iterator const_iterator;

  PrefValueMap();
  virtual ~PrefValueMap();

  
  
  
  bool GetValue(const std::string& key, const Value** value) const;
  bool GetValue(const std::string& key, Value** value);

  
  
  bool SetValue(const std::string& key, Value* value);

  
  
  bool RemoveValue(const std::string& key);

  
  void Clear();

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

  
  
  bool GetBoolean(const std::string& key, bool* value) const;

  
  
  bool GetString(const std::string& key, std::string* value) const;

  
  void SetString(const std::string& key, const std::string& value);

  
  
  
  void GetDifferingKeys(const PrefValueMap* other,
                        std::vector<std::string>* differing_keys) const;

 private:
  typedef std::map<std::string, Value*> Map;

  Map prefs_;

  DISALLOW_COPY_AND_ASSIGN(PrefValueMap);
};

#endif  
