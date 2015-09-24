// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_MANIFEST_HANDLERS_APP_LAUNCH_INFO_H_
#define CHROME_COMMON_EXTENSIONS_MANIFEST_HANDLERS_APP_LAUNCH_INFO_H_

#include <string>
#include <vector>

#include "chrome/common/extensions/extension_constants.h"
#include "extensions/common/extension.h"
#include "extensions/common/manifest.h"
#include "extensions/common/manifest_handler.h"
#include "url/gurl.h"

namespace extensions {

class AppLaunchInfo : public Extension::ManifestData {
 public:
  AppLaunchInfo();
  virtual ~AppLaunchInfo();

  
  static const std::string& GetLaunchLocalPath(const Extension* extension);

  
  static const GURL& GetLaunchWebURL(const Extension* extension);

  
  
  
  
  
  static LaunchContainer GetLaunchContainer(
      const Extension* extension);

  
  
  static int GetLaunchWidth(const Extension* extension);
  static int GetLaunchHeight(const Extension* extension);

  
  static GURL GetFullLaunchURL(const Extension* extension);

  bool Parse(Extension* extension, base::string16* error);

 private:
  bool LoadLaunchURL(Extension* extension, base::string16* error);
  bool LoadLaunchContainer(Extension* extension, base::string16* error);
  void OverrideLaunchURL(Extension* extension, GURL override_url);

  std::string launch_local_path_;

  GURL launch_web_url_;

  LaunchContainer launch_container_;

  int launch_width_;
  int launch_height_;

  DISALLOW_COPY_AND_ASSIGN(AppLaunchInfo);
};

class AppLaunchManifestHandler : public ManifestHandler {
 public:
  AppLaunchManifestHandler();
  virtual ~AppLaunchManifestHandler();

  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;
  virtual bool AlwaysParseForType(Manifest::Type type) const OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(AppLaunchManifestHandler);
};

}  

#endif  
