// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_API_EXTENSION_ACTION_SCRIPT_BADGE_HANDLER_H_
#define CHROME_COMMON_EXTENSIONS_API_EXTENSION_ACTION_SCRIPT_BADGE_HANDLER_H_

#include <string>

#include "chrome/common/extensions/api/extension_action/action_info.h"
#include "extensions/common/extension.h"
#include "extensions/common/manifest_handler.h"

namespace extensions {

class ScriptBadgeHandler : public ManifestHandler {
 public:
  ScriptBadgeHandler();
  virtual ~ScriptBadgeHandler();

  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;
  virtual const std::vector<std::string> PrerequisiteKeys() const OVERRIDE;
  virtual bool AlwaysParseForType(Manifest::Type type) const OVERRIDE;

 private:
  
  
  
  void SetActionInfoDefaults(const Extension* extension, ActionInfo* info);

  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(ScriptBadgeHandler);
};

}  

#endif  
