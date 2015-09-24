// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_MANIFEST_HANDLERS_APP_ISOLATION_INFO_H_
#define CHROME_COMMON_EXTENSIONS_MANIFEST_HANDLERS_APP_ISOLATION_INFO_H_

#include <string>
#include <vector>

#include "extensions/common/extension.h"
#include "extensions/common/manifest.h"
#include "extensions/common/manifest_handler.h"

namespace extensions {

struct AppIsolationInfo : public Extension::ManifestData {
  explicit AppIsolationInfo(bool isolated_storage);
  virtual ~AppIsolationInfo();

  static bool HasIsolatedStorage(const Extension* extension);

  
  bool has_isolated_storage;
};

class AppIsolationHandler : public ManifestHandler {
 public:
  AppIsolationHandler();
  virtual ~AppIsolationHandler();

  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;
  virtual bool AlwaysParseForType(Manifest::Type type) const OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(AppIsolationHandler);
};

}  

#endif  
