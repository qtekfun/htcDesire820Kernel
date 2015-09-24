// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_EXTENSIONS_EXTENSION_SETTINGS_BROWSERTEST_H_
#define CHROME_BROWSER_UI_WEBUI_EXTENSIONS_EXTENSION_SETTINGS_BROWSERTEST_H_

#include "chrome/browser/extensions/extension_test_notification_observer.h"
#include "chrome/test/base/web_ui_browsertest.h"
#include "extensions/common/extension.h"

class Profile;

class ExtensionSettingsUIBrowserTest : public WebUIBrowserTest {
 public:
  ExtensionSettingsUIBrowserTest();
  virtual ~ExtensionSettingsUIBrowserTest();

 protected:
  
  Profile* GetProfile();

  const std::string& last_loaded_extension_id() {
    return observer_->last_loaded_extension_id();
  }

  virtual void SetUpOnMainThread() OVERRIDE;

  void InstallGoodExtension();

 private:
  bool WaitForExtensionViewsToLoad();
  const extensions::Extension* LoadUnpackedExtension(
      const base::FilePath& path);
  const extensions::Extension* InstallExtension(const base::FilePath& path);

  scoped_ptr<ExtensionTestNotificationObserver> observer_;

  
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionSettingsUIBrowserTest);
};

#endif  
