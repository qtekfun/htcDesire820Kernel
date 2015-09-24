// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_MANIFEST_HANDLERS_BACKGROUND_INFO_H_
#define EXTENSIONS_COMMON_MANIFEST_HANDLERS_BACKGROUND_INFO_H_

#include <string>
#include <vector>

#include "base/values.h"
#include "extensions/common/extension.h"
#include "extensions/common/manifest_handler.h"
#include "url/gurl.h"

namespace extensions {

class BackgroundInfo : public Extension::ManifestData {
 public:
  BackgroundInfo();
  virtual ~BackgroundInfo();

  static GURL GetBackgroundURL(const Extension* extension);
  static const std::vector<std::string>& GetBackgroundScripts(
      const Extension* extension);
  static bool HasBackgroundPage(const Extension* extension);
  static bool HasPersistentBackgroundPage(const Extension* extension);
  static bool HasLazyBackgroundPage(const Extension* extension);
  static bool HasGeneratedBackgroundPage(const Extension* extension);
  static bool AllowJSAccess(const Extension* extension);

  bool has_background_page() const {
    return background_url_.is_valid() || !background_scripts_.empty();
  }

  bool has_persistent_background_page() const {
    return has_background_page() && is_persistent_;
  }

  bool has_lazy_background_page() const {
    return has_background_page() && !is_persistent_;
  }

  bool Parse(const Extension* extension, string16* error);

 private:
  bool LoadBackgroundScripts(const Extension* extension,
                             const std::string& key,
                             string16* error);
  bool LoadBackgroundPage(const Extension* extension,
                          const std::string& key,
                          string16* error);
  bool LoadBackgroundPage(const Extension* extension, string16* error);
  bool LoadBackgroundPersistent(const Extension* extension, string16* error);
  bool LoadAllowJSAccess(const Extension* extension, string16* error);

  
  
  GURL background_url_;

  
  
  std::vector<std::string> background_scripts_;

  
  
  bool is_persistent_;

  
  
  
  
  
  bool allow_js_access_;

  DISALLOW_COPY_AND_ASSIGN(BackgroundInfo);
};

class BackgroundManifestHandler : public ManifestHandler {
 public:
  BackgroundManifestHandler();
  virtual ~BackgroundManifestHandler();

  virtual bool Parse(Extension* extension, string16* error) OVERRIDE;
  virtual bool Validate(const Extension* extension,
                        std::string* error,
                        std::vector<InstallWarning>* warnings) const OVERRIDE;
  virtual bool AlwaysParseForType(Manifest::Type type) const OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(BackgroundManifestHandler);
};

}  

#endif  
