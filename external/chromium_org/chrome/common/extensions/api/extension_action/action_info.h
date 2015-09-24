// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_API_EXTENSION_ACTION_ACTION_INFO_H_
#define CHROME_COMMON_EXTENSIONS_API_EXTENSION_ACTION_ACTION_INFO_H_

#include <string>

#include "base/strings/string16.h"
#include "chrome/common/extensions/extension_icon_set.h"
#include "extensions/common/extension.h"
#include "url/gurl.h"

namespace base {
class DictionaryValue;
}

namespace extensions {

class Extension;

struct ActionInfo {
  ActionInfo();
  ~ActionInfo();

  
  enum Type {
    TYPE_BROWSER,
    TYPE_PAGE,
    TYPE_SCRIPT_BADGE,
    TYPE_SYSTEM_INDICATOR,
  };

  
  static scoped_ptr<ActionInfo> Load(const Extension* extension,
                                     const base::DictionaryValue* dict,
                                     base::string16* error);

  
  static const ActionInfo* GetBrowserActionInfo(const Extension* extension);

  
  static const ActionInfo* GetPageActionInfo(const Extension* extension);

  
  static const ActionInfo* GetScriptBadgeInfo(const Extension* etxension);

  
  static const ActionInfo* GetSystemIndicatorInfo(const Extension* extension);

  
  static void SetBrowserActionInfo(Extension* extension, ActionInfo* info);

  
  static void SetPageActionInfo(Extension* extension, ActionInfo* info);

  
  static void SetScriptBadgeInfo(Extension* extension, ActionInfo* info);

  
  
  static void SetSystemIndicatorInfo(Extension* extension, ActionInfo* info);

  
  
  static bool IsVerboseInstallMessage(const Extension* extension);

  
  ExtensionIconSet default_icon;
  std::string default_title;
  GURL default_popup_url;
  
  std::string id;
};

}  

#endif  
