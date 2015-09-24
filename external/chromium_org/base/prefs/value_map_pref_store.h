// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PREFS_VALUE_MAP_PREF_STORE_H_
#define BASE_PREFS_VALUE_MAP_PREF_STORE_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "base/prefs/base_prefs_export.h"
#include "base/prefs/pref_store.h"
#include "base/prefs/pref_value_map.h"

class BASE_PREFS_EXPORT ValueMapPrefStore : public PrefStore {
 public:
  ValueMapPrefStore();

  
  virtual bool GetValue(const std::string& key,
                        const base::Value** value) const OVERRIDE;
  virtual void AddObserver(PrefStore::Observer* observer) OVERRIDE;
  virtual void RemoveObserver(PrefStore::Observer* observer) OVERRIDE;
  virtual bool HasObservers() const OVERRIDE;

 protected:
  virtual ~ValueMapPrefStore();

  
  
  void SetValue(const std::string& key, base::Value* value);

  
  
  void RemoveValue(const std::string& key);

  
  void NotifyInitializationCompleted();

 private:
  PrefValueMap prefs_;

  ObserverList<PrefStore::Observer, true> observers_;

  DISALLOW_COPY_AND_ASSIGN(ValueMapPrefStore);
};

#endif  
