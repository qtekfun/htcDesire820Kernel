// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_EXTENSION_APP_MODEL_BUILDER_H_
#define CHROME_BROWSER_UI_APP_LIST_EXTENSION_APP_MODEL_BUILDER_H_

#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "chrome/browser/extensions/install_observer.h"
#include "ui/app_list/app_list_model.h"
#include "ui/base/models/list_model_observer.h"

class AppListControllerDelegate;
class ExtensionAppItem;
class ExtensionSet;
class Profile;

namespace extensions {
class Extension;
class InstallTracker;
}

namespace gfx {
class ImageSkia;
}

class ExtensionAppModelBuilder : public extensions::InstallObserver,
                                 public app_list::AppListItemListObserver {
 public:
  ExtensionAppModelBuilder(Profile* profile,
                           app_list::AppListModel* model,
                           AppListControllerDelegate* controller);
  virtual ~ExtensionAppModelBuilder();

 private:
  typedef std::vector<ExtensionAppItem*> ExtensionAppList;

  
  void BuildModel();

  
  virtual void OnBeginExtensionInstall(
      const ExtensionInstallParams& params) OVERRIDE;
  virtual void OnDownloadProgress(const std::string& extension_id,
                                  int percent_downloaded) OVERRIDE;
  virtual void OnInstallFailure(const std::string& extension_id) OVERRIDE;
  virtual void OnExtensionInstalled(
      const extensions::Extension* extension) OVERRIDE {}
  virtual void OnExtensionLoaded(
      const extensions::Extension* extension) OVERRIDE;
  virtual void OnExtensionUnloaded(
      const extensions::Extension* extension) OVERRIDE;
  virtual void OnExtensionUninstalled(
      const extensions::Extension* extension) OVERRIDE;
  virtual void OnAppsReordered() OVERRIDE;
  virtual void OnAppInstalledToAppList(
      const std::string& extension_id) OVERRIDE;
  virtual void OnShutdown() OVERRIDE;

  
  virtual void OnListItemMoved(size_t from_index,
                               size_t to_index,
                               app_list::AppListItemModel* item) OVERRIDE;

  
  void AddApps(const ExtensionSet* extensions, ExtensionAppList* apps);

  
  void PopulateApps();

  
  void ResortApps();

  
  void InsertApp(ExtensionAppItem* app);

  
  
  void SetHighlightedApp(const std::string& extension_id);

  
  
  
  
  void UpdateHighlight();

  
  ExtensionAppItem* GetExtensionAppItem(const std::string& extension_id);

  Profile* profile_;

  
  AppListControllerDelegate* controller_;

  
  app_list::AppListModel* model_;

  std::string highlight_app_id_;

  
  
  bool highlighted_app_pending_;

  
  extensions::InstallTracker* tracker_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionAppModelBuilder);
};

#endif  
