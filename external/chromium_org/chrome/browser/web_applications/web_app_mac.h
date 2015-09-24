// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEB_APPLICATIONS_WEB_APP_MAC_H_
#define CHROME_BROWSER_WEB_APPLICATIONS_WEB_APP_MAC_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "chrome/browser/shell_integration.h"
#include "chrome/browser/web_applications/web_app.h"

namespace web_app {

base::FilePath GetAppInstallPath(
    const ShellIntegration::ShortcutInfo& shortcut_info);

void MaybeLaunchShortcut(const ShellIntegration::ShortcutInfo& shortcut_info);

class WebAppShortcutCreator {
 public:
  
  
  
  WebAppShortcutCreator(
      const base::FilePath& app_data_dir,
      const ShellIntegration::ShortcutInfo& shortcut_info);

  virtual ~WebAppShortcutCreator();

  
  base::FilePath GetShortcutBasename() const;

  
  
  virtual base::FilePath GetApplicationsDirname() const;

  
  base::FilePath GetApplicationsShortcutPath() const;

  
  base::FilePath GetInternalShortcutPath() const;

  bool CreateShortcuts(ShortcutCreationReason creation_reason,
                       ShellIntegration::ShortcutLocations creation_locations);
  void DeleteShortcuts();
  bool UpdateShortcuts();

 protected:
  
  
  
  virtual base::FilePath GetAppBundleById(const std::string& bundle_id) const;

  
  virtual void RevealAppShimInFinder() const;

 private:
  FRIEND_TEST_ALL_PREFIXES(WebAppShortcutCreatorTest, DeleteShortcuts);
  FRIEND_TEST_ALL_PREFIXES(WebAppShortcutCreatorTest, UpdateIcon);
  FRIEND_TEST_ALL_PREFIXES(WebAppShortcutCreatorTest, UpdateShortcuts);

  
  std::string GetBundleIdentifier() const;

  
  std::string GetInternalBundleIdentifier() const;

  
  
  bool BuildShortcut(const base::FilePath& staging_path) const;

  
  
  size_t CreateShortcutsIn(const std::vector<base::FilePath>& folders) const;

  
  
  bool UpdateDisplayName(const base::FilePath& app_path) const;

  
  bool UpdateInternalBundleIdentifier() const;

  
  bool UpdatePlist(const base::FilePath& app_path) const;

  
  bool UpdateIcon(const base::FilePath& app_path) const;

  
  
  base::FilePath app_data_dir_;

  
  ShellIntegration::ShortcutInfo info_;

  DISALLOW_COPY_AND_ASSIGN(WebAppShortcutCreator);
};

}  

#endif  
