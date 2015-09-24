// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_BROWSER_EXTENSION_PREFS_SCOPE_H_
#define EXTENSIONS_BROWSER_EXTENSION_PREFS_SCOPE_H_

#include "base/basictypes.h"

namespace extensions {

enum ExtensionPrefsScope {
  
  kExtensionPrefsScopeRegular,
  
  kExtensionPrefsScopeRegularOnly,
  
  
  kExtensionPrefsScopeIncognitoPersistent,
  
  
  kExtensionPrefsScopeIncognitoSessionOnly
};

}  

#endif  
