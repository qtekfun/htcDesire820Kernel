// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_BROWSERTEST_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_BROWSERTEST_H_

#include <string>

#include "base/command_line.h"

#include "base/files/file_path.h"
#include "base/files/scoped_temp_dir.h"
#include "chrome/browser/extensions/extension_host.h"
#include "chrome/browser/extensions/extension_system.h"
#include "chrome/browser/extensions/extension_test_notification_observer.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/common/extensions/features/feature_channel.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "content/public/browser/web_contents.h"
#include "extensions/common/extension.h"
#include "extensions/common/feature_switch.h"
#include "extensions/common/manifest.h"

class ExtensionService;
class ExtensionSet;
class Profile;

namespace extensions {
class ProcessManager;
}

class ExtensionBrowserTest : virtual public InProcessBrowserTest {
 protected:
  
  enum Flags {
    kFlagNone = 0,

    
    kFlagEnableIncognito = 1 << 0,

    
    kFlagEnableFileAccess = 1 << 1,

    
    
    kFlagIgnoreManifestWarnings = 1 << 2,

    
    
    kFlagAllowOldManifestVersions = 1 << 3,
  };

  ExtensionBrowserTest();
  virtual ~ExtensionBrowserTest();

  
  ExtensionService* extension_service() {
    return extensions::ExtensionSystem::Get(profile())->extension_service();
  }

  const std::string& last_loaded_extension_id() {
    return observer_->last_loaded_extension_id();
  }

  
  Profile* profile();

  static const extensions::Extension* GetExtensionByPath(
      const ExtensionSet* extensions, const base::FilePath& path);

  
  virtual void SetUpCommandLine(CommandLine* command_line) OVERRIDE;
  virtual void SetUpOnMainThread() OVERRIDE;

  const extensions::Extension* LoadExtension(const base::FilePath& path);

  
  const extensions::Extension* LoadExtensionIncognito(
      const base::FilePath& path);

  const extensions::Extension* LoadExtensionWithFlags(
      const base::FilePath& path, int flags);

  
  
  
  const extensions::Extension* LoadExtensionAsComponentWithManifest(
      const base::FilePath& path,
      const base::FilePath::CharType* manifest_relative_path);

  
  
  
  const extensions::Extension* LoadExtensionAsComponent(
      const base::FilePath& path);

  
  
  base::FilePath PackExtension(const base::FilePath& dir_path);

  
  
  
  
  base::FilePath PackExtensionWithOptions(const base::FilePath& dir_path,
                                          const base::FilePath& crx_path,
                                          const base::FilePath& pem_path,
                                          const base::FilePath& pem_out_path);

  
  
  
  
  const extensions::Extension* InstallExtension(const base::FilePath& path,
                                                int expected_change) {
    return InstallOrUpdateExtension(
        std::string(), path, INSTALL_UI_TYPE_NONE, expected_change);
  }

  
  
  const extensions::Extension* InstallExtension(
      const base::FilePath& path,
      int expected_change,
      extensions::Manifest::Location install_source) {
    return InstallOrUpdateExtension(std::string(),
                                    path,
                                    INSTALL_UI_TYPE_NONE,
                                    expected_change,
                                    install_source);
  }

  
  const extensions::Extension* InstallExtensionFromWebstore(
      const base::FilePath& path, int expected_change);

  
  
  const extensions::Extension* UpdateExtension(const std::string& id,
                                               const base::FilePath& path,
                                               int expected_change) {
    return InstallOrUpdateExtension(id, path, INSTALL_UI_TYPE_NONE,
                                    expected_change);
  }

  
  const extensions::Extension* UpdateExtensionWaitForIdle(
      const std::string& id, const base::FilePath& path, int expected_change);

  
  
  const extensions::Extension* InstallExtensionWithUI(
      const base::FilePath& path,
      int expected_change) {
    return InstallOrUpdateExtension(
        std::string(), path, INSTALL_UI_TYPE_NORMAL, expected_change);
  }

  const extensions::Extension* InstallExtensionWithUIAutoConfirm(
      const base::FilePath& path,
      int expected_change,
      Browser* browser) {
    return InstallOrUpdateExtension(std::string(),
                                    path,
                                    INSTALL_UI_TYPE_AUTO_CONFIRM,
                                    expected_change,
                                    browser,
                                    extensions::Extension::NO_FLAGS);
  }

  const extensions::Extension* InstallExtensionWithSourceAndFlags(
      const base::FilePath& path,
      int expected_change,
      extensions::Manifest::Location install_source,
      extensions::Extension::InitFromValueFlags creation_flags) {
    return InstallOrUpdateExtension(std::string(), path, INSTALL_UI_TYPE_NONE,
        expected_change, install_source, browser(), creation_flags, false);
  }

  
  const extensions::Extension* StartInstallButCancel(
      const base::FilePath& path) {
    return InstallOrUpdateExtension(
        std::string(), path, INSTALL_UI_TYPE_CANCEL, 0);
  }

  void ReloadExtension(const std::string extension_id);

  void UnloadExtension(const std::string& extension_id);

  void UninstallExtension(const std::string& extension_id);

  void DisableExtension(const std::string& extension_id);

  void EnableExtension(const std::string& extension_id);

  
  bool WaitForPageActionCountChangeTo(int count) {
    return observer_->WaitForPageActionCountChangeTo(count);
  }

  
  bool WaitForPageActionVisibilityChangeTo(int count) {
    return observer_->WaitForPageActionVisibilityChangeTo(count);
  }

  
  
  bool WaitForExtensionInstall() {
    return observer_->WaitForExtensionInstall();
  }

  
  
  bool WaitForExtensionInstallError() {
    return observer_->WaitForExtensionInstallError();
  }

  
  void WaitForExtensionAndViewLoad() {
    return observer_->WaitForExtensionAndViewLoad();
  }

  
  void WaitForExtensionLoad() {
    return observer_->WaitForExtensionLoad();
  }

  
  
  bool WaitForExtensionLoadError() {
    return observer_->WaitForExtensionLoadError();
  }

  
  
  bool WaitForExtensionCrash(const std::string& extension_id) {
    return observer_->WaitForExtensionCrash(extension_id);
  }

  
  bool WaitForCrxInstallerDone() {
    return observer_->WaitForCrxInstallerDone();
  }

  
  bool WaitForExtensionViewsToLoad() {
    return observer_->WaitForExtensionViewsToLoad();
  }

  
  
  void OpenWindow(content::WebContents* contents,
                  const GURL& url,
                  bool newtab_process_should_equal_opener,
                  content::WebContents** newtab_result);

  
  
  void NavigateInRenderer(content::WebContents* contents, const GURL& url);

  
  
  
  extensions::ExtensionHost* FindHostWithPath(
      extensions::ProcessManager* manager,
      const std::string& path,
      int expected_hosts);

  
  
  
  std::string ExecuteScriptInBackgroundPage(const std::string& extension_id,
                                            const std::string& script);

  bool loaded_;
  bool installed_;

  
  base::FilePath test_data_dir_;

  scoped_ptr<ExtensionTestNotificationObserver> observer_;

 private:
  
  base::ScopedTempDir temp_dir_;

  
  
  enum InstallUIType {
    INSTALL_UI_TYPE_NONE,
    INSTALL_UI_TYPE_CANCEL,
    INSTALL_UI_TYPE_NORMAL,
    INSTALL_UI_TYPE_AUTO_CONFIRM,
  };

  const extensions::Extension* InstallOrUpdateExtension(
      const std::string& id,
      const base::FilePath& path,
      InstallUIType ui_type,
      int expected_change);
  const extensions::Extension* InstallOrUpdateExtension(
      const std::string& id,
      const base::FilePath& path,
      InstallUIType ui_type,
      int expected_change,
      Browser* browser,
      extensions::Extension::InitFromValueFlags creation_flags);
  const extensions::Extension* InstallOrUpdateExtension(
      const std::string& id,
      const base::FilePath& path,
      InstallUIType ui_type,
      int expected_change,
      extensions::Manifest::Location install_source);
  const extensions::Extension* InstallOrUpdateExtension(
      const std::string& id,
      const base::FilePath& path,
      InstallUIType ui_type,
      int expected_change,
      extensions::Manifest::Location install_source,
      Browser* browser,
      extensions::Extension::InitFromValueFlags creation_flags,
      bool wait_for_idle);

  
  extensions::ScopedCurrentChannel current_channel_;

  
  extensions::FeatureSwitch::ScopedOverride
      override_prompt_for_external_extensions_;

  
  Profile* profile_;
};

#endif  
