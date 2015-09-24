// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_SCOPED_PREFS_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_SCOPED_PREFS_H_

namespace extensions {

class ExtensionScopedPrefs {
 public:
  ExtensionScopedPrefs() {}
  ~ExtensionScopedPrefs() {}

  
  virtual void UpdateExtensionPref(const std::string& id,
                                   const std::string& key,
                                   base::Value* value) = 0;

  
  virtual void DeleteExtensionPrefs(const std::string& id) = 0;

  
  virtual bool ReadPrefAsBoolean(const std::string& extension_id,
                                 const std::string& pref_key,
                                 bool* out_value) const = 0;

  
  virtual bool ReadPrefAsInteger(const std::string& extension_id,
                                 const std::string& pref_key,
                                 int* out_value) const = 0;

  
  virtual bool ReadPrefAsString(const std::string& extension_id,
                                const std::string& pref_key,
                                std::string* out_value) const = 0;

  
  virtual bool ReadPrefAsList(const std::string& extension_id,
                              const std::string& pref_key,
                              const base::ListValue** out_value) const = 0;

  
  virtual bool ReadPrefAsDictionary(
      const std::string& extension_id,
      const std::string& pref_key,
      const base::DictionaryValue** out_value) const = 0;

  
  
  virtual bool HasPrefForExtension(const std::string& extension_id) const = 0;
};

}  

#endif  
