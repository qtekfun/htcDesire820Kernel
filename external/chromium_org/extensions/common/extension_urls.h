// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_EXTENSION_URLS_H_
#define EXTENSIONS_COMMON_EXTENSION_URLS_H_

#include "base/strings/string16.h"

namespace extensions {

extern const char kEventBindings[];

extern const char kSchemaUtils[];

bool IsSourceFromAnExtension(const base::string16& source);

}  

#endif  
