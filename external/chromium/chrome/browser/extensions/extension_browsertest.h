// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_BROWSERTEST_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_BROWSERTEST_H_
#pragma once

#include <string>

#include "base/command_line.h"
#include "base/file_path.h"
#include "base/memory/scoped_temp_dir.h"
#include "chrome/test/in_process_browser_test.h"
#include "content/common/notification_details.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_type.h"

class Extension;

class ExtensionBrowserTest
    : public InProcessBrowserTest, public NotificationObserver {
 protected:
  ExtensionBrowserTest();

  virtual void SetUpCommandLine(CommandLine* command_line);
  const Extension* LoadExtension(const FilePath& path);

  
  const Extension* LoadExtensionIncognito(const FilePath& path);

  
  
  const Extension* LoadExtensionNoFileAccess(const FilePath& path);

  
  const Extension* LoadExtensionIncognitoNoFileAccess(const FilePath& path);

  
  bool LoadExtensionAsComponent(const FilePath& path);

  
  
  FilePath PackExtension(const FilePath& dir_path);

  
  
  
  
  bool InstallExtension(const FilePath& path, int expected_change) {
    return InstallOrUpdateExtension("", path, INSTALL_UI_TYPE_NONE,
                                    expected_change);
  }

  
  
  bool UpdateExtension(const std::string& id, const FilePath& path,
                       int expected_change) {
    return InstallOrUpdateExtension(id, path, INSTALL_UI_TYPE_NONE,
                                    expected_change);
  }

  
  
  bool InstallExtensionWithUI(const FilePath& path, int expected_change) {
    return InstallOrUpdateExtension("", path, INSTALL_UI_TYPE_NORMAL,
                                    expected_change);
  }
  bool InstallExtensionWithUIAutoConfirm(const FilePath& path,
                                         int expected_change,
                                         Profile* profile) {
    return InstallOrUpdateExtension("", path, INSTALL_UI_TYPE_AUTO_CONFIRM,
                                    expected_change, profile);
  }

  
  bool StartInstallButCancel(const FilePath& path) {
    return InstallOrUpdateExtension("", path, INSTALL_UI_TYPE_CANCEL, 0);
  }

  void ReloadExtension(const std::string& extension_id);

  void UnloadExtension(const std::string& extension_id);

  void UninstallExtension(const std::string& extension_id);

  void DisableExtension(const std::string& extension_id);

  void EnableExtension(const std::string& extension_id);

  
  bool WaitForPageActionCountChangeTo(int count);

  
  bool WaitForPageActionVisibilityChangeTo(int count);

  
  
  bool WaitForExtensionInstall();

  
  
  bool WaitForExtensionInstallError();

  
  void WaitForExtensionLoad();

  
  
  bool WaitForExtensionCrash(const std::string& extension_id);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  bool loaded_;
  bool installed_;

  
  FilePath test_data_dir_;
  std::string last_loaded_extension_id_;
  int extension_installs_observed_;

 private:
  
  ScopedTempDir temp_dir_;

  
  
  enum InstallUIType {
    INSTALL_UI_TYPE_NONE,
    INSTALL_UI_TYPE_CANCEL,
    INSTALL_UI_TYPE_NORMAL,
    INSTALL_UI_TYPE_AUTO_CONFIRM,
  };

  bool InstallOrUpdateExtension(const std::string& id, const FilePath& path,
                                InstallUIType ui_type,
                                int expected_change);
  bool InstallOrUpdateExtension(const std::string& id, const FilePath& path,
                                InstallUIType ui_type,
                                int expected_change,
                                Profile* profile);
  const Extension* LoadExtensionImpl(const FilePath& path,
                                     bool incognito_enabled,
                                     bool fileaccess_enabled);

  bool WaitForExtensionHostsToLoad();

  
  
  int target_page_action_count_;

  
  
  int target_visible_page_action_count_;
};

#endif  
