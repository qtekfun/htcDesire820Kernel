// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_GOOGLE_CHROME_DISTRIBUTION_H_
#define CHROME_INSTALLER_UTIL_GOOGLE_CHROME_DISTRIBUTION_H_

#include "base/gtest_prod_util.h"
#include "base/strings/string16.h"
#include "chrome/installer/util/browser_distribution.h"

namespace base {
class FilePath;
}

class GoogleChromeDistribution : public BrowserDistribution {
 public:
  
  
  
  
  
  
  
  
  
  virtual void DoPostUninstallOperations(
      const Version& version,
      const base::FilePath& local_data_path,
      const string16& distribution_data) OVERRIDE;

  virtual string16 GetActiveSetupGuid() OVERRIDE;

  virtual string16 GetAppGuid() OVERRIDE;

  virtual string16 GetShortcutName(ShortcutType shortcut_type) OVERRIDE;

  virtual string16 GetIconFilename() OVERRIDE;

  virtual int GetIconIndex(ShortcutType shortcut_type) OVERRIDE;

  virtual string16 GetBaseAppName() OVERRIDE;

  virtual string16 GetBaseAppId() OVERRIDE;

  virtual string16 GetBrowserProgIdPrefix() OVERRIDE;

  virtual string16 GetBrowserProgIdDesc() OVERRIDE;

  virtual string16 GetInstallSubDir() OVERRIDE;

  virtual string16 GetPublisherName() OVERRIDE;

  virtual string16 GetAppDescription() OVERRIDE;

  virtual std::string GetSafeBrowsingName() OVERRIDE;

  virtual string16 GetStateKey() OVERRIDE;

  virtual string16 GetStateMediumKey() OVERRIDE;

  virtual std::string GetNetworkStatsServer() const OVERRIDE;

  virtual std::string GetHttpPipeliningTestServer() const OVERRIDE;

  
  
  
  virtual string16 GetDistributionData(HKEY root_key) OVERRIDE;

  virtual string16 GetUninstallLinkName() OVERRIDE;

  virtual string16 GetUninstallRegPath() OVERRIDE;

  virtual string16 GetVersionKey() OVERRIDE;

  virtual bool GetCommandExecuteImplClsid(
      string16* handler_class_uuid) OVERRIDE;

  virtual bool AppHostIsSupported() OVERRIDE;

  virtual void UpdateInstallStatus(
      bool system_install,
      installer::ArchiveType archive_type,
      installer::InstallStatus install_status) OVERRIDE;

  virtual bool ShouldSetExperimentLabels() OVERRIDE;

  virtual bool HasUserExperiments() OVERRIDE;

  const string16& product_guid() { return product_guid_; }

 protected:
  void set_product_guid(const string16& guid) { product_guid_ = guid; }

  
  GoogleChromeDistribution();

 private:
  friend class BrowserDistribution;

  
  string16 product_guid_;
};

#endif  
