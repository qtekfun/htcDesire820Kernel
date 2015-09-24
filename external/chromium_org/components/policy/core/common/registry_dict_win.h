// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_REGISTRY_DICT_WIN_H_
#define COMPONENTS_POLICY_CORE_COMMON_REGISTRY_DICT_WIN_H_

#include <windows.h>

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "components/policy/policy_export.h"

namespace base {
class Value;
}

namespace policy {

class Schema;

struct POLICY_EXPORT CaseInsensitiveStringCompare {
  bool operator()(const std::string& a, const std::string& b) const;
};

class POLICY_EXPORT RegistryDict {
 public:
  typedef std::map<std::string, RegistryDict*,
      CaseInsensitiveStringCompare> KeyMap;
  typedef std::map<std::string, base::Value*,
      CaseInsensitiveStringCompare> ValueMap;

  RegistryDict();
  ~RegistryDict();

  
  RegistryDict* GetKey(const std::string& name);
  const RegistryDict* GetKey(const std::string& name) const;
  
  void SetKey(const std::string& name, scoped_ptr<RegistryDict> dict);
  
  scoped_ptr<RegistryDict> RemoveKey(const std::string& name);
  
  void ClearKeys();

  
  base::Value* GetValue(const std::string& name);
  const base::Value* GetValue(const std::string& name) const;
  
  void SetValue(const std::string& name, scoped_ptr<base::Value> value);
  
  scoped_ptr<base::Value> RemoveValue(const std::string& name);
  
  void ClearValues();

  
  void Merge(const RegistryDict& other);

  
  void Swap(RegistryDict* other);

  
  void ReadRegistry(HKEY hive, const base::string16& root);

  
  
  
  
  scoped_ptr<base::Value> ConvertToJSON(const Schema& schema) const;

  const KeyMap& keys() const { return keys_; }
  const ValueMap& values() const { return values_; }

 private:
  KeyMap keys_;
  ValueMap values_;

  DISALLOW_COPY_AND_ASSIGN(RegistryDict);
};

}  

#endif  
