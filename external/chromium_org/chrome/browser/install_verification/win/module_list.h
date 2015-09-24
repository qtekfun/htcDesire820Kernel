// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INSTALL_VERIFICATION_WIN_MODULE_LIST_H_
#define CHROME_BROWSER_INSTALL_VERIFICATION_WIN_MODULE_LIST_H_

#include <Windows.h>

#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

struct ModuleInfo;

class ModuleList {
 public:
  ~ModuleList();

  
  
  
  
  
  static scoped_ptr<ModuleList> FromLoadedModuleSnapshot(
      const std::vector<HMODULE>& snapshot);

  
  void GetModuleInfoSet(std::set<ModuleInfo>* module_info_set);

  size_t size() const {
    return modules_.size();
  }

  HMODULE operator[](size_t index) const {
    return modules_[index];
  }

 private:
  ModuleList();

  std::vector<HMODULE> modules_;

  DISALLOW_COPY_AND_ASSIGN(ModuleList);
};

#endif  
