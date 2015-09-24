// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PREFS_DEFAULT_PREF_STORE_H_
#define BASE_PREFS_DEFAULT_PREF_STORE_H_

#include <string>

#include "base/observer_list.h"
#include "base/prefs/base_prefs_export.h"
#include "base/prefs/pref_store.h"
#include "base/prefs/pref_value_map.h"
#include "base/values.h"

class BASE_PREFS_EXPORT DefaultPrefStore : public PrefStore {
 public:
  typedef PrefValueMap::const_iterator const_iterator;

  DefaultPrefStore();

  
  virtual bool GetValue(const std::string& key,
                        const base::Value** result) const OVERRIDE;
  virtual void AddObserver(PrefStore::Observer* observer) OVERRIDE;
  virtual void RemoveObserver(PrefStore::Observer* observer) OVERRIDE;
  virtual bool HasObservers() const OVERRIDE;

  
  
  void SetDefaultValue(const std::string& key, scoped_ptr<base::Value> value);

  
  
  void ReplaceDefaultValue(const std::string& key,
                           scoped_ptr<base::Value> value);

  const_iterator begin() const;
  const_iterator end() const;

 private:
  virtual ~DefaultPrefStore();

  PrefValueMap prefs_;

  ObserverList<PrefStore::Observer, true> observers_;

  DISALLOW_COPY_AND_ASSIGN(DefaultPrefStore);
};

#endif  
