// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_API_OMNIBOX_OMNIBOX_HANDLER_H_
#define CHROME_COMMON_EXTENSIONS_API_OMNIBOX_OMNIBOX_HANDLER_H_

#include <string>

#include "extensions/common/extension.h"
#include "extensions/common/manifest_handler.h"

namespace extensions {

class Extension;

struct OmniboxInfo : public Extension::ManifestData {
  
  std::string keyword;

  
  static const std::string& GetKeyword(const Extension* extension);
};

class OmniboxHandler : public ManifestHandler {
 public:
  OmniboxHandler();
  virtual ~OmniboxHandler();

  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;
};

}  

#endif  
