// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PREFS_PREF_REGISTRY_H_
#define BASE_PREFS_PREF_REGISTRY_H_

#include "base/memory/ref_counted.h"
#include "base/prefs/base_prefs_export.h"
#include "base/prefs/pref_value_map.h"

namespace base {
class Value;
}

class DefaultPrefStore;
class PrefStore;

class BASE_PREFS_EXPORT PrefRegistry : public base::RefCounted<PrefRegistry> {
 public:
  typedef PrefValueMap::const_iterator const_iterator;

  PrefRegistry();

  
  scoped_refptr<PrefStore> defaults();

  
  const_iterator begin() const;
  const_iterator end() const;

  
  
  
  void SetDefaultPrefValue(const char* pref_name, base::Value* value);

 protected:
  friend class base::RefCounted<PrefRegistry>;
  virtual ~PrefRegistry();

  
  void RegisterPreference(const char* path, base::Value* default_value);

  scoped_refptr<DefaultPrefStore> defaults_;

 private:
  DISALLOW_COPY_AND_ASSIGN(PrefRegistry);
};

#endif  
