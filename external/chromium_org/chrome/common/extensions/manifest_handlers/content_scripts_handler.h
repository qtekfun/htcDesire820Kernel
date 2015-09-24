// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_MANIFEST_HANDLERS_CONTENT_SCRIPTS_HANDLER_H_
#define CHROME_COMMON_EXTENSIONS_MANIFEST_HANDLERS_CONTENT_SCRIPTS_HANDLER_H_

#include <string>

#include "extensions/common/extension.h"
#include "extensions/common/manifest_handler.h"
#include "extensions/common/user_script.h"

namespace extensions {

class URLPatternSet;

struct ContentScriptsInfo : public Extension::ManifestData {
  ContentScriptsInfo();
  virtual ~ContentScriptsInfo();

  
  UserScriptList content_scripts;

  
  
  static const UserScriptList& GetContentScripts(const Extension* extension);

  
  static URLPatternSet GetScriptableHosts(const Extension* extension);

  
  static bool ExtensionHasScriptAtURL(const Extension* extension,
                                      const GURL& url);
};

class ContentScriptsHandler : public ManifestHandler {
 public:
  ContentScriptsHandler();
  virtual ~ContentScriptsHandler();

  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;
  virtual bool Validate(const Extension* extension,
                        std::string* error,
                        std::vector<InstallWarning>* warnings) const OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(ContentScriptsHandler);
};

}  

#endif  
