// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_BROWSER_DISTRIBUTION_H_
#define CHROME_INSTALLER_UTIL_BROWSER_DISTRIBUTION_H_

#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/strings/string16.h"
#include "base/version.h"
#include "chrome/installer/util/util_constants.h"

#if defined(OS_WIN)
#include <windows.h>  
#endif

class BrowserDistribution {
 public:
  enum Type {
    CHROME_BROWSER,
    CHROME_FRAME,
    CHROME_BINARIES,
    CHROME_APP_HOST,
    NUM_TYPES
  };

  enum ShortcutType {
    SHORTCUT_CHROME,
    SHORTCUT_CHROME_ALTERNATE,
    SHORTCUT_APP_LAUNCHER
  };

  enum Subfolder {
    SUBFOLDER_CHROME,
    SUBFOLDER_APPS,
  };

  enum DefaultBrowserControlPolicy {
    DEFAULT_BROWSER_UNSUPPORTED,
    DEFAULT_BROWSER_OS_CONTROL_ONLY,
    DEFAULT_BROWSER_FULL_CONTROL
  };

  virtual ~BrowserDistribution() {}

  static BrowserDistribution* GetDistribution();

  static BrowserDistribution* GetSpecificDistribution(Type type);

  Type GetType() const { return type_; }

  virtual void DoPostUninstallOperations(const Version& version,
                                         const base::FilePath& local_data_path,
                                         const string16& distribution_data);

  
  virtual string16 GetActiveSetupGuid();

  virtual string16 GetAppGuid();

  
  
  
  
  
  virtual string16 GetBaseAppName();

  
  virtual string16 GetDisplayName();

  
  
  virtual string16 GetShortcutName(ShortcutType shortcut_type);

  
  
  virtual int GetIconIndex(ShortcutType shortcut_type);

  
  virtual string16 GetIconFilename();

  
  
  
  virtual string16 GetStartMenuShortcutSubfolder(Subfolder subfolder_type);

  
  
  
  
  virtual string16 GetBaseAppId();

  
  
  
  
  
  
  
  virtual string16 GetBrowserProgIdPrefix();

  
  virtual string16 GetBrowserProgIdDesc();

  virtual string16 GetInstallSubDir();

  virtual string16 GetPublisherName();

  virtual string16 GetAppDescription();

  virtual string16 GetLongAppDescription();

  virtual std::string GetSafeBrowsingName();

  virtual string16 GetStateKey();

  virtual string16 GetStateMediumKey();

  virtual std::string GetNetworkStatsServer() const;

  virtual std::string GetHttpPipeliningTestServer() const;

#if defined(OS_WIN)
  virtual string16 GetDistributionData(HKEY root_key);
#endif

  virtual string16 GetUninstallLinkName();

  virtual string16 GetUninstallRegPath();

  virtual string16 GetVersionKey();

  
  
  virtual DefaultBrowserControlPolicy GetDefaultBrowserControlPolicy();

  virtual bool CanCreateDesktopShortcuts();

  virtual bool GetChromeChannel(string16* channel);

  
  
  
  virtual bool GetCommandExecuteImplClsid(string16* handler_class_uuid);

  
  virtual bool AppHostIsSupported();

  virtual void UpdateInstallStatus(bool system_install,
      installer::ArchiveType archive_type,
      installer::InstallStatus install_status);

  
  
  virtual bool ShouldSetExperimentLabels();

  virtual bool HasUserExperiments();

 protected:
  explicit BrowserDistribution(Type type);

  template<class DistributionClass>
  static BrowserDistribution* GetOrCreateBrowserDistribution(
      BrowserDistribution** dist);

  const Type type_;

 private:
  BrowserDistribution();

  DISALLOW_COPY_AND_ASSIGN(BrowserDistribution);
};

#endif  
