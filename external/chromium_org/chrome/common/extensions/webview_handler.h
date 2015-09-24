// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_WEBVIEW_HANDLER_H_
#define CHROME_COMMON_EXTENSIONS_WEBVIEW_HANDLER_H_

#include <string>

#include "extensions/common/extension.h"
#include "extensions/common/manifest_handler.h"

namespace extensions {

struct WebviewInfo : public Extension::ManifestData {
  
  WebviewInfo();
  virtual ~WebviewInfo();

  
  static bool IsResourceWebviewAccessible(const Extension* extension,
                                          const std::string& partition_id,
                                          const std::string& relative_path);

  
  
  static bool HasWebviewAccessibleResources(const Extension* extension);

  
  std::vector<std::string> webview_privileged_partitions_;
  URLPatternSet webview_accessible_resources_;
};

class WebviewHandler : public ManifestHandler {
 public:
  WebviewHandler();
  virtual ~WebviewHandler();

  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(WebviewHandler);
};

}  

#endif  
