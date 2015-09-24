// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_STORAGE_SETTINGS_NAMESPACE_H_
#define CHROME_BROWSER_EXTENSIONS_API_STORAGE_SETTINGS_NAMESPACE_H_

#include <string>

namespace extensions {

namespace settings_namespace {

enum Namespace {
  LOCAL,    
  SYNC,     
  MANAGED,  
  INVALID
};

std::string ToString(Namespace settings_namespace);

Namespace FromString(const std::string& ns_string);

}  

}  

#endif  
