// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_FUNCTION_UTIL_H__
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_FUNCTION_UTIL_H__

#include <vector>
#include "base/values.h"

namespace base {
class Value;
}

namespace extensions {

bool ReadOneOrMoreIntegers(base::Value* value, std::vector<int>* result);

} 

#endif  
