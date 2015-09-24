// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_PERSISTENT_PREF_STORE_H_
#define CHROME_COMMON_PERSISTENT_PREF_STORE_H_
#pragma once

#include <string>

#include <chrome/common/pref_store.h>

class PersistentPrefStore : public PrefStore {
 public:
  virtual ~PersistentPrefStore() {}

  
  
  
  
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
    PREF_READ_ERROR_FILE_NOT_SPECIFIED
  };

  
  virtual ReadResult GetMutableValue(const std::string& key,
                                     Value** result) = 0;

  
  
  
  
  virtual void ReportValueChanged(const std::string& key) = 0;

  
  
  virtual void SetValue(const std::string& key, Value* value) = 0;

  
  
  
  
  virtual void SetValueSilently(const std::string& key, Value* value) = 0;

  
  virtual void RemoveValue(const std::string& key) = 0;

  
  
  
  virtual bool ReadOnly() const = 0;

  
  virtual PrefReadError ReadPrefs() = 0;

  
  virtual bool WritePrefs() = 0;

  
  virtual void ScheduleWritePrefs() = 0;

  
  virtual void CommitPendingWrite() = 0;
};

#endif  
