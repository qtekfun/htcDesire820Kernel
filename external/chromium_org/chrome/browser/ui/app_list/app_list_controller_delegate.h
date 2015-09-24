// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_APP_LIST_CONTROLLER_DELEGATE_H_
#define CHROME_BROWSER_UI_APP_LIST_APP_LIST_CONTROLLER_DELEGATE_H_

#include <string>

#include "chrome/common/extensions/extension_constants.h"
#include "ui/gfx/native_widget_types.h"

class ExtensionSet;
class Profile;

namespace base {
class FilePath;
}

namespace extensions {
class Extension;
class InstallTracker;
}

namespace gfx {
class ImageSkia;
}

class AppListControllerDelegate {
 public:
  
  enum AppListSource {
    LAUNCH_FROM_UNKNOWN,
    LAUNCH_FROM_APP_LIST,
    LAUNCH_FROM_APP_LIST_SEARCH
  };

  
  enum Pinnable {
    NO_PIN,
    PIN_EDITABLE,
    PIN_FIXED
  };

  virtual ~AppListControllerDelegate();

  
  
  virtual bool ForceNativeDesktop() const;

  
  virtual void DismissView() = 0;

  
  virtual void ViewClosing();

  
  virtual gfx::NativeWindow GetAppListWindow() = 0;

  
  virtual gfx::ImageSkia GetWindowIcon() = 0;

  
  virtual bool IsAppPinned(const std::string& extension_id) = 0;
  virtual void PinApp(const std::string& extension_id) = 0;
  virtual void UnpinApp(const std::string& extension_id) = 0;
  virtual Pinnable GetPinnable() = 0;

  
  virtual void OnShowExtensionPrompt();
  virtual void OnCloseExtensionPrompt();

  
  virtual bool CanDoCreateShortcutsFlow() = 0;

  
  
  virtual void DoCreateShortcutsFlow(Profile* profile,
                                     const std::string& extension_id) = 0;

  
  
  virtual void CreateNewWindow(Profile* profile, bool incognito) = 0;

  
  virtual void ActivateApp(Profile* profile,
                           const extensions::Extension* extension,
                           AppListSource source,
                           int event_flags) = 0;

  
  virtual void LaunchApp(Profile* profile,
                         const extensions::Extension* extension,
                         AppListSource source,
                         int event_flags) = 0;

  
  virtual void ShowForProfileByPath(const base::FilePath& profile_path) = 0;

  
  
  virtual bool ShouldShowUserIcon() = 0;

  static std::string AppListSourceToString(AppListSource source);

  
  bool UserMayModifySettings(Profile* profile, const std::string& app_id);

  
  void UninstallApp(Profile* profile, const std::string& app_id);

  
  bool IsAppFromWebStore(Profile* profile,
                         const std::string& app_id);

  
  void ShowAppInWebStore(Profile* profile,
                         const std::string& app_id,
                         bool is_search_result);

  
  bool HasOptionsPage(Profile* profile, const std::string& app_id);

  
  void ShowOptionsPage(Profile* profile, const std::string& app_id);

  
  
  
  extensions::LaunchType GetExtensionLaunchType(
      Profile* profile, const std::string& app_id);
  virtual void SetExtensionLaunchType(
      Profile* profile,
      const std::string& extension_id,
      extensions::LaunchType launch_type);

  
  bool IsExtensionInstalled(Profile* profile, const std::string& app_id);

  extensions::InstallTracker* GetInstallTrackerFor(Profile* profile);

  
  void GetApps(Profile* profile, ExtensionSet* out_apps);
};

#endif  
