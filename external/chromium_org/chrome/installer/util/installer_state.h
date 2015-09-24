// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_INSTALLER_STATE_H_
#define CHROME_INSTALLER_UTIL_INSTALLER_STATE_H_

#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/version.h"
#include "chrome/installer/util/browser_distribution.h"
#include "chrome/installer/util/product.h"
#include "chrome/installer/util/util_constants.h"

#if defined(OS_WIN)
#include <windows.h>  
#endif

class CommandLine;

namespace installer {

class ChannelInfo;
class InstallationState;
class MasterPreferences;

class ProductState;

typedef std::vector<Product*> Products;

class InstallerState {
 public:
  enum Level {
    UNKNOWN_LEVEL,
    USER_LEVEL,
    SYSTEM_LEVEL
  };

  enum PackageType {
    UNKNOWN_PACKAGE_TYPE,
    SINGLE_PACKAGE,
    MULTI_PACKAGE
  };

  enum Operation {
    UNINITIALIZED,
    SINGLE_INSTALL_OR_UPDATE,
    MULTI_INSTALL,
    MULTI_UPDATE,
    UNINSTALL
  };

  
  InstallerState();

  
  explicit InstallerState(Level level);

  
  void Initialize(const CommandLine& command_line,
                  const MasterPreferences& prefs,
                  const InstallationState& machine_state);

  
  
  
  
  Product* AddProductFromState(BrowserDistribution::Type type,
                               const ProductState& state);

  
  
  
  Product* AddProduct(scoped_ptr<Product>* product);

  
  
  
  bool RemoveProduct(const Product* product);

  
  Level level() const { return level_; }

  
  PackageType package_type() const { return package_type_; }

  
  Operation operation() const { return operation_; }

  
  
  bool system_install() const;

  
  
  bool is_multi_install() const;

  
  
  bool ensure_google_update_present() const {
    return ensure_google_update_present_;
  }

  
  const base::FilePath& target_path() const { return target_path_; }

  
  
  bool is_msi() const { return msi_; }

  
  
  bool verbose_logging() const { return verbose_logging_; }

#if defined(OS_WIN)
  HKEY root_key() const { return root_key_; }
#endif

  
  const std::wstring& state_key() const { return state_key_; }

  
  
  BrowserDistribution::Type state_type() const { return state_type_; }

  
  
  BrowserDistribution* multi_package_binaries_distribution() const {
    DCHECK(package_type_ == MULTI_PACKAGE);
    DCHECK(multi_package_distribution_ != NULL);
    return multi_package_distribution_;
  }

  const Products& products() const { return products_.get(); }

  
  const Product* FindProduct(BrowserDistribution::Type distribution_type) const;

  
  
  base::Version* GetCurrentVersion(
      const InstallationState& machine_state) const;

  
  
  
  
  
  base::Version DetermineCriticalVersion(
      const base::Version* current_version,
      const base::Version& new_version) const;

  
  
  
  bool IsChromeFrameRunning(const InstallationState& machine_state) const;

  
  
  bool AreBinariesInUse(const InstallationState& machine_state) const;

  
  
  base::FilePath GetInstallerDirectory(const base::Version& version) const;

  
  
  
  void RemoveOldVersionDirectories(const base::Version& new_version,
                                   base::Version* existing_version,
                                   const base::FilePath& temp_path) const;

  
  
  void AddComDllList(std::vector<base::FilePath>* com_dll_list) const;

  bool SetChannelFlags(bool set, ChannelInfo* channel_info) const;

  
  void UpdateStage(installer::InstallerStage stage) const;

  
  
  void UpdateChannels() const;

  
  
  
  // |status| itself is written to the InstallerError value.
  // |string_resource_id|, if non-zero, identifies a localized string written to
  
  // non-empty, is written to the InstallerSuccessLaunchCmdLine value.
  void WriteInstallerResult(InstallStatus status,
                            int string_resource_id,
                            const std::wstring* launch_cmd) const;

  
  bool RequiresActiveSetup() const;

 protected:
  
  enum {
    CHROME_DLL              = 1 << 0,
    CHROME_FRAME_DLL        = 1 << 1,
    CHROME_FRAME_HELPER_DLL = 1 << 2,
    CHROME_FRAME_HELPER_EXE = 1 << 3,
    NUM_BINARIES            = 4
  };

  
  
  static bool IsFileInUse(const base::FilePath& file);

  
  void Clear();

  
  
  bool AnyExistsAndIsInUse(const InstallationState& machine_state,
                           uint32 file_bits) const;
  base::FilePath GetDefaultProductInstallPath(BrowserDistribution* dist) const;
  bool CanAddProduct(const Product& product,
                     const base::FilePath* product_dir) const;
  Product* AddProductInDirectory(const base::FilePath* product_dir,
                                 scoped_ptr<Product>* product);
  Product* AddProductFromPreferences(
      BrowserDistribution::Type distribution_type,
      const MasterPreferences& prefs,
      const InstallationState& machine_state);
  bool IsMultiInstallUpdate(const MasterPreferences& prefs,
                            const InstallationState& machine_state);

  
  
  
  void GetExistingExeVersions(std::set<std::string>* existing_versions) const;

  
  void set_level(Level level);

  
  
  void set_package_type(PackageType type);

  Operation operation_;
  base::FilePath target_path_;
  std::wstring state_key_;
  BrowserDistribution::Type state_type_;
  ScopedVector<Product> products_;
  BrowserDistribution* multi_package_distribution_;
  base::Version critical_update_version_;
  Level level_;
  PackageType package_type_;
#if defined(OS_WIN)
  HKEY root_key_;
#endif
  bool msi_;
  bool verbose_logging_;
  bool ensure_google_update_present_;

 private:
  DISALLOW_COPY_AND_ASSIGN(InstallerState);
};  

}  

#endif  
