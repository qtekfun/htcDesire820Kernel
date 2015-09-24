// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PREFS_PERSISTENT_PREF_STORE_H_
#define BASE_PREFS_PERSISTENT_PREF_STORE_H_

#include <string>

#include "base/prefs/base_prefs_export.h"
#include "base/prefs/pref_store.h"

class BASE_PREFS_EXPORT PersistentPrefStore : public PrefStore {
 public:
  
  
  
  
  enum PrefReadError {
    PREF_READ_ERROR_NONE = 0,
    PREF_READ_ERROR_JSON_PARSE,
    PREF_READ_ERROR_JSON_TYPE,
    PREF_READ_ERROR_ACCESS_DENIED,
    PREF_READ_ERROR_FILE_OTHER,
    PREF_READ_ERROR_FILE_LOCKED,
    PREF_READ_ERROR_NO_FILE,
    PREF_READ_ERROR_JSON_REPEAT,
    PREF_READ_ERROR_OTHER,
    PREF_READ_ERROR_FILE_NOT_SPECIFIED,
    PREF_READ_ERROR_MAX_ENUM
  };

  class ReadErrorDelegate {
   public:
    virtual ~ReadErrorDelegate() {}

    virtual void OnError(PrefReadError error) = 0;
  };

  
  virtual bool GetMutableValue(const std::string& key,
                               base::Value** result) = 0;

  
  
  
  
  virtual void ReportValueChanged(const std::string& key) = 0;

  
  
  virtual void SetValue(const std::string& key, base::Value* value) = 0;

  
  
  
  
  virtual void SetValueSilently(const std::string& key, base::Value* value) = 0;

  
  virtual void RemoveValue(const std::string& key) = 0;

  
  
  
  virtual bool ReadOnly() const = 0;

  
  virtual PrefReadError GetReadError() const = 0;

  
  
  virtual PrefReadError ReadPrefs() = 0;

  
  
  
  
  virtual void ReadPrefsAsync(ReadErrorDelegate* error_delegate) = 0;

  
  virtual void CommitPendingWrite() = 0;

 protected:
  virtual ~PersistentPrefStore() {}
};

#endif  
