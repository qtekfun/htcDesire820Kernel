// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_SCOPED_USER_PREF_UPDATE_H_
#define CHROME_BROWSER_PREFS_SCOPED_USER_PREF_UPDATE_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/values.h"
#include "chrome/browser/prefs/pref_service.h"

class DictionaryValue;
class ListValue;
class PrefService;

namespace subtle {

class ScopedUserPrefUpdateBase : public base::NonThreadSafe {
 protected:
  ScopedUserPrefUpdateBase(PrefService* service, const char* path);

  
  virtual ~ScopedUserPrefUpdateBase();

  
  Value* Get(Value::ValueType type);

 private:
  
  
  virtual void Notify();

  
  PrefService* service_;
  
  std::string path_;
  
  Value* value_;

  DISALLOW_COPY_AND_ASSIGN(ScopedUserPrefUpdateBase);
};

}  

template <typename T, Value::ValueType type_enum_value>
class ScopedUserPrefUpdate : public subtle::ScopedUserPrefUpdateBase {
 public:
  ScopedUserPrefUpdate(PrefService* service, const char* path)
      : ScopedUserPrefUpdateBase(service, path) {}

  
  virtual ~ScopedUserPrefUpdate() {}

  
  
  // - is (silently) created and written to the user pref store if none existed
  
  
  
  
  
  
  virtual T* Get() {
    return static_cast<T*>(
        subtle::ScopedUserPrefUpdateBase::Get(type_enum_value));
  }

  T& operator*() {
    return *Get();
  }

  T* operator->() {
    return Get();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ScopedUserPrefUpdate);
};

typedef ScopedUserPrefUpdate<DictionaryValue, Value::TYPE_DICTIONARY>
    DictionaryPrefUpdate;
typedef ScopedUserPrefUpdate<ListValue, Value::TYPE_LIST> ListPrefUpdate;

#endif  
