// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_VALUE_MAP_PREF_STORE_H_
#define CHROME_BROWSER_PREFS_VALUE_MAP_PREF_STORE_H_
#pragma once

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "chrome/browser/prefs/pref_value_map.h"
#include "chrome/common/pref_store.h"

class ValueMapPrefStore : public PrefStore {
 public:
  typedef std::map<std::string, Value*>::iterator iterator;
  typedef std::map<std::string, Value*>::const_iterator const_iterator;

  ValueMapPrefStore();
  virtual ~ValueMapPrefStore();

  
  virtual ReadResult GetValue(const std::string& key,
                              const Value** value) const;
  virtual void AddObserver(PrefStore::Observer* observer);
  virtual void RemoveObserver(PrefStore::Observer* observer);

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

 protected:
  
  
  void SetValue(const std::string& key, Value* value);

  
  
  void RemoveValue(const std::string& key);

  
  void NotifyInitializationCompleted();

 private:
  PrefValueMap prefs_;

  ObserverList<PrefStore::Observer, true> observers_;

  DISALLOW_COPY_AND_ASSIGN(ValueMapPrefStore);
};

#endif  
