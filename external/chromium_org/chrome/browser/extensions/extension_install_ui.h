// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_INSTALL_UI_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_INSTALL_UI_H_

#include <string>

#include "base/basictypes.h"

class Browser;
class ExtensionInstallPrompt;
class Profile;
class SkBitmap;

namespace extensions {
class CrxInstallerError;
class Extension;
class ExtensionWebstorePrivateApiTest;
}

class ExtensionInstallUI {
 public:
  static ExtensionInstallUI* Create(Profile* profile);

  virtual ~ExtensionInstallUI();

  
  virtual void OnInstallSuccess(const extensions::Extension* extension,
                                SkBitmap* icon) = 0;

  
  virtual void OnInstallFailure(const extensions::CrxInstallerError& error) = 0;


  
  
  
  
  virtual void SetUseAppInstalledBubble(bool use_bubble) = 0;

  
  void set_skip_post_install_ui(bool skip_ui) {
    skip_post_install_ui_ = skip_ui;
  }

  
  static void OpenAppInstalledUI(Profile* profile, const std::string& app_id);

#if defined(UNIT_TEST)
  static void set_disable_failure_ui_for_tests() {
    disable_failure_ui_for_tests_ = true;
  }
#endif

  
  
  static ExtensionInstallPrompt* CreateInstallPromptWithBrowser(
      Browser* browser);

  
  
  
  static ExtensionInstallPrompt* CreateInstallPromptWithProfile(
      Profile* profile);

  Profile* profile() { return profile_; }

 protected:
  explicit ExtensionInstallUI(Profile* profile);

  static bool disable_failure_ui_for_tests() {
    return disable_failure_ui_for_tests_;
  }

  bool skip_post_install_ui() const { return skip_post_install_ui_; }

 private:
  static bool disable_failure_ui_for_tests_;

  Profile* profile_;

  
  bool skip_post_install_ui_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionInstallUI);
};

#endif  
