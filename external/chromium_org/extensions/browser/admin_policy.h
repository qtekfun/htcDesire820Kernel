// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_BROWSER_ADMIN_POLICY_H_
#define EXTENSIONS_BROWSER_ADMIN_POLICY_H_

#include "base/values.h"

namespace extensions {

class Extension;

namespace admin_policy {

bool BlacklistedByDefault(const base::ListValue* blacklist);

bool UserMayLoad(const base::ListValue* blacklist,
                 const base::ListValue* whitelist,
                 const base::DictionaryValue* forcelist,
                 const base::ListValue* allowed_types,
                 const Extension* extension,
                 string16* error);

bool UserMayModifySettings(const Extension* extension, string16* error);

bool MustRemainEnabled(const Extension* extension, string16* error);

}  
}  

#endif  
