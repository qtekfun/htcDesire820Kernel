// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_MANIFEST_HANDLERS_MINIMUM_CHROME_VERSION_CHECKER_H_
#define CHROME_COMMON_EXTENSIONS_MANIFEST_HANDLERS_MINIMUM_CHROME_VERSION_CHECKER_H_

#include "extensions/common/manifest_handler.h"

namespace extensions {

class MinimumChromeVersionChecker : public ManifestHandler {
 public:
  MinimumChromeVersionChecker();
  virtual ~MinimumChromeVersionChecker();

  
  
  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(MinimumChromeVersionChecker);
};

}  

#endif  
