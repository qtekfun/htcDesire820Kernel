// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_INSTALL_OBSERVER_H_
#define CHROME_BROWSER_EXTENSIONS_INSTALL_OBSERVER_H_

#include <string>

#include "ui/gfx/image/image_skia.h"

namespace extensions {

class Extension;

class InstallObserver {
 public:
  struct ExtensionInstallParams {
    ExtensionInstallParams(
        std::string extension_id,
        std::string extension_name,
        gfx::ImageSkia installing_icon,
        bool is_app,
        bool is_platform_app);

    std::string extension_id;
    std::string extension_name;
    gfx::ImageSkia installing_icon;
    bool is_app;
    bool is_platform_app;
    bool is_ephemeral;
  };

  virtual void OnBeginExtensionInstall(
      const ExtensionInstallParams& params) = 0;

  virtual void OnDownloadProgress(const std::string& extension_id,
                                  int percent_downloaded) = 0;

  virtual void OnInstallFailure(const std::string& extension_id) = 0;

  virtual void OnExtensionInstalled(const Extension* extension) = 0;
  virtual void OnExtensionLoaded(const Extension* extension) = 0;
  virtual void OnExtensionUnloaded(const Extension* extension) = 0;
  virtual void OnExtensionUninstalled(const Extension* extension) = 0;
  virtual void OnAppsReordered() = 0;
  virtual void OnAppInstalledToAppList(const std::string& extension_id) = 0;

  
  virtual void OnShutdown() = 0;

 protected:
  virtual ~InstallObserver() {}
};

}  

#endif  