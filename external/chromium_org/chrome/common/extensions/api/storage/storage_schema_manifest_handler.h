// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_API_STORAGE_STORAGE_SCHEMA_MANIFEST_HANDLER_H_
#define CHROME_COMMON_EXTENSIONS_API_STORAGE_STORAGE_SCHEMA_MANIFEST_HANDLER_H_

#include "base/basictypes.h"
#include "extensions/common/manifest_handler.h"

namespace policy {
class Schema;
}

namespace extensions {

class StorageSchemaManifestHandler : public ManifestHandler {
 public:
  StorageSchemaManifestHandler();
  virtual ~StorageSchemaManifestHandler();

#if defined(ENABLE_CONFIGURATION_POLICY)
  
  
  
  
  static policy::Schema GetSchema(const Extension* extension,
                                  std::string* error);
#endif

 private:
  
  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;
  virtual bool Validate(const Extension* extension,
                        std::string* error,
                        std::vector<InstallWarning>* warnings) const OVERRIDE;
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(StorageSchemaManifestHandler);
};

}  

#endif  
