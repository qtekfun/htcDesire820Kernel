// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_INSTALLER_TEST_PE_IMAGE_RESOURCES_H_
#define CHROME_INSTALLER_TEST_PE_IMAGE_RESOURCES_H_

#include <windows.h>

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/logging.h"

namespace base { namespace win { class PEImage; } }

namespace upgrade_test {

class EntryId {
 public:
  explicit EntryId(WORD number) : number_(number) { }
  explicit EntryId(const std::wstring& name) : name_(name), number_() {
    DCHECK_NE(static_cast<std::wstring::size_type>(0), name.size());
  }
  bool IsNamed() const { return !name_.empty(); }
  WORD number() const { return number_; }
  const std::wstring& name() const { return name_; }
 private:
  std::wstring name_;
  WORD number_;
};  

typedef std::vector<EntryId> EntryPath;

typedef void (*EnumResource_Fn)(const EntryPath& path, uint8* data,
                                DWORD size, DWORD code_page, uintptr_t context);

bool EnumResources(const base::win::PEImage& image, EnumResource_Fn callback,
                   uintptr_t context);

}  

#endif  
