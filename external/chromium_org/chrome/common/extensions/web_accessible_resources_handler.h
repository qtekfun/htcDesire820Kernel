// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_WEB_ACCESSIBLE_RESOURCES_HANDLER_H_
#define CHROME_COMMON_EXTENSIONS_WEB_ACCESSIBLE_RESOURCES_HANDLER_H_

#include <string>

#include "extensions/common/extension.h"
#include "extensions/common/manifest_handler.h"

namespace extensions {

struct WebAccessibleResourcesInfo : public Extension::ManifestData {
  
  WebAccessibleResourcesInfo();
  virtual ~WebAccessibleResourcesInfo();

  
  static bool IsResourceWebAccessible(const Extension* extension,
                                      const std::string& relative_path);

  
  static bool HasWebAccessibleResources(const Extension* extension);

  
  URLPatternSet web_accessible_resources_;
};

class WebAccessibleResourcesHandler : public ManifestHandler {
 public:
  WebAccessibleResourcesHandler();
  virtual ~WebAccessibleResourcesHandler();

  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(WebAccessibleResourcesHandler);
};

}  

#endif  
