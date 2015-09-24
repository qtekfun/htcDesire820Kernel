// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_PRODUCT_OPERATIONS_H_
#define CHROME_INSTALLER_UTIL_PRODUCT_OPERATIONS_H_

#include <set>
#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/strings/string16.h"
#include "chrome/installer/util/shell_util.h"
#include "chrome/installer/util/util_constants.h"

class BrowserDistribution;
class CommandLine;

namespace installer {

class ChannelInfo;
class MasterPreferences;

class ProductOperations {
 public:
  virtual ~ProductOperations() {}

  
  virtual void ReadOptions(const MasterPreferences& prefs,
                           std::set<string16>* options) const = 0;

  
  virtual void ReadOptions(const CommandLine& command,
                           std::set<string16>* options) const = 0;

  
  
  
  
  
  
  
  
  virtual void AddKeyFiles(const std::set<string16>& options,
                           std::vector<base::FilePath>* key_files) const = 0;

  
  
  virtual void AddComDllList(
      const std::set<string16>& options,
      std::vector<base::FilePath>* com_dll_list) const = 0;

  
  
  
  virtual void AppendProductFlags(const std::set<string16>& options,
                                  CommandLine* cmd_line) const = 0;

  
  virtual void AppendRenameFlags(const std::set<string16>& options,
                                 CommandLine* cmd_line) const = 0;

  
  
  virtual bool SetChannelFlags(const std::set<string16>& options,
                               bool set,
                               ChannelInfo* channel_info) const = 0;

  
  
  
  virtual bool ShouldCreateUninstallEntry(
      const std::set<string16>& options) const = 0;

  
  
  virtual void AddDefaultShortcutProperties(
      BrowserDistribution* dist,
      const base::FilePath& target_exe,
      ShellUtil::ShortcutProperties* properties) const = 0;

  
  
  
  virtual void LaunchUserExperiment(const base::FilePath& setup_path,
                                    const std::set<string16>& options,
                                    InstallStatus status,
                                    bool system_level) const = 0;
};

}  

#endif  
