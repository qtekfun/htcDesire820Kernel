// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PREFS_PREF_VALUE_MAP_H_
#define BASE_PREFS_PREF_VALUE_MAP_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/prefs/base_prefs_export.h"

namespace base {
class Value;
}

class BASE_PREFS_EXPORT PrefValueMap {
 public:
  typedef std::map<std::string, base::Value*>::iterator iterator;
  typedef std::map<std::string, base::Value*>::const_iterator const_iterator;

  PrefValueMap();
  virtual ~PrefValueMap();

  
  
  
  bool GetValue(const std::string& key, const base::Value** value) const;
  bool GetValue(const std::string& key, base::Value** value);

  
  
  bool SetValue(const std::string& key, base::Value* value);

  
  
  bool RemoveValue(const std::string& key);

  
  void Clear();

  
  void Swap(PrefValueMap* other);

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

  
  
  bool GetBoolean(const std::string& key, bool* value) const;

  
  void SetBoolean(const std::string& key, bool value);

  
  
  bool GetString(const std::string& key, std::string* value) const;

  
  void SetString(const std::string& key, const std::string& value);

  
  
  bool GetInteger(const std::string& key, int* value) const;

  
  void SetInteger(const std::string& key, const int value);

  
  
  
  void GetDifferingKeys(const PrefValueMap* other,
                        std::vector<std::string>* differing_keys) const;

 private:
  typedef std::map<std::string, base::Value*> Map;

  Map prefs_;

  DISALLOW_COPY_AND_ASSIGN(PrefValueMap);
};

#endif  
