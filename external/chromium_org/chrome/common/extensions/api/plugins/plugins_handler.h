// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_API_PLUGINS_PLUGINS_HANDLER_H_
#define CHROME_COMMON_EXTENSIONS_API_PLUGINS_PLUGINS_HANDLER_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "extensions/common/extension.h"
#include "extensions/common/manifest_handler.h"

namespace extensions {

struct PluginInfo {
  typedef std::vector<PluginInfo> PluginVector;

  PluginInfo(const base::FilePath& plugin_path, bool plugin_is_public);
  ~PluginInfo();

  base::FilePath path;  
  bool is_public;  

  
  static const PluginVector* GetPlugins(const Extension* extension);

  
  static bool HasPlugins(const Extension* extension);
};

class PluginsHandler : public ManifestHandler {
 public:
  PluginsHandler();
  virtual ~PluginsHandler();

  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;
  virtual bool Validate(const Extension* extension,
                        std::string* error,
                        std::vector<InstallWarning>* warnings) const OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(PluginsHandler);
};

}  

#endif  
