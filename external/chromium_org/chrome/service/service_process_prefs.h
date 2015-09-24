// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_SERVICE_SERVICE_PROCESS_PREFS_H_
#define CHROME_SERVICE_SERVICE_PROCESS_PREFS_H_

#include <string>

#include "base/prefs/json_pref_store.h"

namespace base {
class DictionaryValue;
class ListValue;
class SequencedTaskRunner;
}

class ServiceProcessPrefs {
 public:
  
  ServiceProcessPrefs(const base::FilePath& pref_filename,
                      base::SequencedTaskRunner* task_runner);
  ~ServiceProcessPrefs();

  
  void ReadPrefs();

  
  void WritePrefs();

  
  std::string GetString(const std::string& key,
                        const std::string& default_value) const;

  
  void SetString(const std::string& key, const std::string& value);

  
  bool GetBoolean(const std::string& key, bool default_value) const;

  
  void SetBoolean(const std::string& key, bool value);

  
  int GetInt(const std::string& key, int default_value) const;

  
  void SetInt(const std::string& key, int value);

  
  const base::DictionaryValue* GetDictionary(const std::string& key) const;

  
  const base::ListValue* GetList(const std::string& key) const;

  
  void SetValue(const std::string& key, base::Value* value);

  
  void RemovePref(const std::string& key);

 private:
  scoped_refptr<JsonPrefStore> prefs_;

  DISALLOW_COPY_AND_ASSIGN(ServiceProcessPrefs);
};

#endif  
