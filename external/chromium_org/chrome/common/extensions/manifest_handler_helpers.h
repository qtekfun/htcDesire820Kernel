// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_MANIFEST_HANDLER_HELPERS_H_
#define CHROME_COMMON_EXTENSIONS_MANIFEST_HANDLER_HELPERS_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"

class ExtensionIconSet;

namespace base {
class DictionaryValue;
}

namespace extensions {
namespace manifest_handler_helpers {

bool NormalizeAndValidatePath(std::string* path);

bool LoadIconsFromDictionary(const base::DictionaryValue* icons_value,
                             const int* icon_sizes,
                             size_t num_icon_sizes,
                             ExtensionIconSet* icons,
                             base::string16* error);

}  
}  

#endif  
