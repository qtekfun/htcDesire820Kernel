// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INSTALL_VERIFICATION_WIN_MODULE_INFO_H_
#define CHROME_BROWSER_INSTALL_VERIFICATION_WIN_MODULE_INFO_H_

#include "base/strings/string16.h"

struct ModuleInfo {
  ModuleInfo() : base_address(0), size(0) {
  }

  ModuleInfo(const base::char16* const module_name,
             uintptr_t module_base_address,
             size_t module_size)
      : base_address(module_base_address),
        size(module_size),
        name(module_name) {
  }

  
  bool operator< (const ModuleInfo& compare) const {
    return base_address < compare.base_address;
  }

  
  bool ContainsAddress(uintptr_t address) const {
    return address >= base_address && address < base_address + size;
  }

  base::string16 name;
  uintptr_t  base_address;
  size_t size;
};

#endif  
