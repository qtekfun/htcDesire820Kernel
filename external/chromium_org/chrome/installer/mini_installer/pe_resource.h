// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_MINI_INSTALLER_PE_RESOURCE_H_
#define CHROME_INSTALLER_MINI_INSTALLER_PE_RESOURCE_H_

#include <windows.h>

class PEResource {
 public:
  
  
  PEResource(HRSRC resource, HMODULE module);

  
  
  PEResource(const wchar_t* name, const wchar_t* type, HMODULE module);

  
  bool IsValid();

  
  
  size_t Size();

  
  // not be loaded into memory or if it cannot be written to disk it returns
  
  bool WriteToDisk(const wchar_t* path);

 private:
  HRSRC resource_;
  HMODULE module_;
};

#endif  
