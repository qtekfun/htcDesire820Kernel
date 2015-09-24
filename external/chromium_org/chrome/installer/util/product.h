// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_PRODUCT_H_
#define CHROME_INSTALLER_UTIL_PRODUCT_H_

#include <set>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "chrome/installer/util/browser_distribution.h"
#include "chrome/installer/util/shell_util.h"
#include "chrome/installer/util/util_constants.h"

class CommandLine;

namespace installer {

class ChannelInfo;
class MasterPreferences;
class Product;
class ProductOperations;

class Product {
 public:
  explicit Product(BrowserDistribution* distribution);

  ~Product();

  void InitializeFromPreferences(const MasterPreferences& prefs);

  void InitializeFromUninstallCommand(const CommandLine& uninstall_command);

  BrowserDistribution* distribution() const {
    return distribution_;
  }

  bool is_type(BrowserDistribution::Type type) const {
    return distribution_->GetType() == type;
  }

  bool is_chrome() const {
    return distribution_->GetType() == BrowserDistribution::CHROME_BROWSER;
  }

  bool is_chrome_frame() const {
    return distribution_->GetType() == BrowserDistribution::CHROME_FRAME;
  }

  bool is_chrome_app_host() const {
    return distribution_->GetType() == BrowserDistribution::CHROME_APP_HOST;
  }

  bool is_chrome_binaries() const {
    return distribution_->GetType() == BrowserDistribution::CHROME_BINARIES;
  }

  bool HasOption(const std::wstring& option) const {
    return options_.find(option) != options_.end();
  }

  
  bool SetOption(const std::wstring& option, bool set) {
    if (set)
      return options_.insert(option).second;
    else
      return options_.erase(option) != 0;
  }

  
  
  
  
  
  
  
  
  void GetUserDataPaths(std::vector<base::FilePath>* paths) const;

  
  bool LaunchChrome(const base::FilePath& application_path) const;

  
  
  
  
  
  
  bool LaunchChromeAndWait(const base::FilePath& application_path,
                           const CommandLine& options,
                           int32* exit_code) const;

  
  
  
  bool SetMsiMarker(bool system_install, bool set) const;

  
  
  bool ShouldCreateUninstallEntry() const;

  
  void AddKeyFiles(std::vector<base::FilePath>* key_files) const;

  
  void AddComDllList(std::vector<base::FilePath>* com_dll_list) const;

  
  void AppendProductFlags(CommandLine* command_line) const;

  
  void AppendRenameFlags(CommandLine* command_line) const;

  
  bool SetChannelFlags(bool set, ChannelInfo* channel_info) const;

  
  void AddDefaultShortcutProperties(
      const base::FilePath& target_exe,
      ShellUtil::ShortcutProperties* properties) const;

  void LaunchUserExperiment(const base::FilePath& setup_path,
                            InstallStatus status,
                            bool system_level) const;

 protected:
  enum CacheStateFlags {
    MSI_STATE = 0x01
  };

  BrowserDistribution* distribution_;
  scoped_ptr<ProductOperations> operations_;
  std::set<std::wstring> options_;

 private:
  DISALLOW_COPY_AND_ASSIGN(Product);
};

}  

#endif  
