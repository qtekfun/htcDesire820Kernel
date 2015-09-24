// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_API_IDENTITY_OAUTH2_MANIFEST_HANDLER_H_
#define CHROME_COMMON_EXTENSIONS_API_IDENTITY_OAUTH2_MANIFEST_HANDLER_H_

#include <set>
#include <string>
#include <vector>

#include "extensions/common/extension.h"
#include "extensions/common/manifest_handler.h"

namespace extensions {

struct OAuth2Info : public Extension::ManifestData {
  OAuth2Info();
  virtual ~OAuth2Info();

  std::string client_id;
  std::vector<std::string> scopes;

  
  bool auto_approve;

  static const OAuth2Info& GetOAuth2Info(const Extension* extension);
};

class OAuth2ManifestHandler : public ManifestHandler {
 public:
  OAuth2ManifestHandler();
  virtual ~OAuth2ManifestHandler();

  virtual bool Parse(Extension* extension,
                     base::string16* error) OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(OAuth2ManifestHandler);
};

}  

#endif  
