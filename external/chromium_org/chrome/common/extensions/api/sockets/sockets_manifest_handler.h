// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_API_SOCKETS_SOCKETS_MANIFEST_HANDLER_H_
#define CHROME_COMMON_EXTENSIONS_API_SOCKETS_SOCKETS_MANIFEST_HANDLER_H_

#include <string>
#include <vector>

#include "extensions/common/manifest_handler.h"

namespace extensions {
class Extension;
class ManifestPermission;
}

namespace extensions {

class SocketsManifestHandler : public ManifestHandler {
 public:
  SocketsManifestHandler();
  virtual ~SocketsManifestHandler();

  
  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;
  virtual ManifestPermission* CreatePermission() OVERRIDE;
  virtual ManifestPermission* CreateInitialRequiredPermission(
    const Extension* extension) OVERRIDE;

 private:
  
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(SocketsManifestHandler);
};

}  

#endif  
