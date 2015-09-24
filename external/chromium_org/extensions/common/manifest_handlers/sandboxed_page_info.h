// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_MANIFEST_HANDLERS_SANDBOXED_PAGE_INFO_H_
#define EXTENSIONS_COMMON_MANIFEST_HANDLERS_SANDBOXED_PAGE_INFO_H_

#include <string>

#include "extensions/common/extension.h"
#include "extensions/common/manifest_handler.h"
#include "extensions/common/url_pattern_set.h"

namespace extensions {

struct SandboxedPageInfo : public Extension::ManifestData {
 public:
  SandboxedPageInfo();
  virtual ~SandboxedPageInfo();

  
  static const std::string& GetContentSecurityPolicy(
      const Extension* extension);

  
  static const URLPatternSet& GetPages(const Extension* extension);

  
  static bool IsSandboxedPage(const Extension* extension,
                              const std::string& relative_path);

  
  
  URLPatternSet pages;

  
  
  
  std::string content_security_policy;
};

class SandboxedPageHandler : public ManifestHandler {
 public:
  SandboxedPageHandler();
  virtual ~SandboxedPageHandler();

  virtual bool Parse(Extension* extension, string16* error) OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;
};

}  

#endif  
