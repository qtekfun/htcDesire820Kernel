// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_MASTER_PREFERENCES_H_
#define CHROME_INSTALLER_UTIL_MASTER_PREFERENCES_H_

#include <string>
#include <vector>

#include "base/command_line.h"
#include "base/memory/scoped_ptr.h"

namespace base {
class DictionaryValue;
class FilePath;
}

namespace installer {

#if !defined(OS_MACOSX)
const char kDefaultMasterPrefs[] = "master_preferences";
#endif


class MasterPreferences {
 public:
  
  
  
  MasterPreferences();

  
  
  
  
  
  explicit MasterPreferences(const CommandLine& cmd_line);

  
  
  explicit MasterPreferences(const base::FilePath& prefs_path);

  
  
  explicit MasterPreferences(const std::string& prefs);

  ~MasterPreferences();

  
  
  
  bool GetBool(const std::string& name, bool* value) const;
  bool GetInt(const std::string& name, int* value) const;
  bool GetString(const std::string& name, std::string* value) const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  std::vector<std::string> GetFirstRunTabs() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool GetExtensionsBlock(base::DictionaryValue** extensions) const;

  
  std::string GetVariationsSeed() const;

  
  bool read_from_file() const {
    return preferences_read_from_file_;
  }

  bool install_chrome() const {
    return chrome_;
  }

  bool install_chrome_app_launcher() const {
    return chrome_app_launcher_;
  }

  bool is_multi_install() const {
    return multi_install_;
  }

  
  const base::DictionaryValue& master_dictionary() const {
    return *master_dictionary_.get();
  }

  
  
  
  
  static const MasterPreferences& ForCurrentProcess();

 protected:
  void InitializeFromCommandLine(const CommandLine& cmd_line);

  
  
  bool InitializeFromString(const std::string& json_data);

  void InitializeProductFlags();

  
  
  void EnforceLegacyPreferences();

 protected:
  scoped_ptr<base::DictionaryValue> master_dictionary_;
  base::DictionaryValue* distribution_;
  bool preferences_read_from_file_;
  bool chrome_;
  bool chrome_app_launcher_;
  bool multi_install_;

 private:
  DISALLOW_COPY_AND_ASSIGN(MasterPreferences);
};

}  

#endif  
