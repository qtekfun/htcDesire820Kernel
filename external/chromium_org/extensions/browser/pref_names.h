// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_BROWSER_PREF_NAMES_H_
#define EXTENSIONS_BROWSER_PREF_NAMES_H_

#include <string>

#include "extensions/browser/extension_prefs_scope.h"

namespace extensions {

namespace pref_names {

bool ScopeToPrefName(ExtensionPrefsScope scope, std::string* result);

extern const char kPrefPreferences[];

extern const char kPrefIncognitoPreferences[];

extern const char kPrefRegularOnlyPreferences[];

extern const char kPrefContentSettings[];

extern const char kPrefIncognitoContentSettings[];

}  

}  

#endif  
