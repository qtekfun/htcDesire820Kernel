// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_INSTALLATION_STATE_H_
#define CHROME_INSTALLER_UTIL_INSTALLATION_STATE_H_

#include <string>

#include "base/basictypes.h"
#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/installer/util/app_commands.h"
#include "chrome/installer/util/browser_distribution.h"
#include "chrome/installer/util/channel_info.h"

namespace base {
class Version;
namespace win {
class RegKey;
}
}

namespace installer {

class InstallationState;

class ProductState {
 public:
  ProductState();

  
  
  bool Initialize(bool system_install,
                  BrowserDistribution::Type type);
  bool Initialize(bool system_install,
                  BrowserDistribution* distribution);

  
  const ChannelInfo& channel() const { return channel_; }

  
  base::FilePath GetSetupPath() const;

  
  
  const base::Version& version() const;

  
  
  
  const base::Version* old_version() const { return old_version_.get(); }

  
  const std::wstring& brand() const { return brand_; }

  
  
  const std::wstring& rename_cmd() const { return rename_cmd_; }

  
  
  
  
  bool GetEulaAccepted(DWORD* eula_accepted) const;

  
  
  
  bool GetOemInstall(std::wstring* oem_install) const;

  
  
  
  
  bool GetUsageStats(DWORD* usagestats) const;

  
  bool is_msi() const { return msi_; }

  
  const CommandLine& uninstall_command() const { return uninstall_command_; }

  
  bool is_multi_install() const { return multi_install_; }

  
  const AppCommands& commands() const { return commands_; }

  
  ProductState& CopyFrom(const ProductState& other);

  
  void Clear();

 protected:
  static bool InitializeCommands(const base::win::RegKey& version_key,
                                 AppCommands* commands);

  ChannelInfo channel_;
  scoped_ptr<Version> version_;
  scoped_ptr<Version> old_version_;
  std::wstring brand_;
  std::wstring rename_cmd_;
  std::wstring oem_install_;
  CommandLine uninstall_command_;
  AppCommands commands_;
  DWORD eula_accepted_;
  DWORD usagestats_;
  bool msi_ : 1;
  bool multi_install_ : 1;
  bool has_eula_accepted_ : 1;
  bool has_oem_install_ : 1;
  bool has_usagestats_ : 1;

 private:
  friend class InstallationState;

  DISALLOW_COPY_AND_ASSIGN(ProductState);
};  

class InstallationState {
 public:
  InstallationState();

  
  void Initialize();

  
  
  const ProductState* GetProductState(bool system_install,
                                      BrowserDistribution::Type type) const;

  
  
  // information has been written by Omaha. Notably absent from the
  
  
  
  
  const ProductState* GetNonVersionedProductState(
      bool system_install, BrowserDistribution::Type type) const;

 protected:
  enum {
    CHROME_BROWSER_INDEX,
    CHROME_FRAME_INDEX,
    CHROME_BINARIES_INDEX,
    CHROME_APP_HOST_INDEX,
    NUM_PRODUCTS
  };

  static int IndexFromDistType(BrowserDistribution::Type type);

  ProductState user_products_[NUM_PRODUCTS];
  ProductState system_products_[NUM_PRODUCTS];

 private:
  DISALLOW_COPY_AND_ASSIGN(InstallationState);
};  

}  

#endif  
