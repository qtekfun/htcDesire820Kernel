// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_APP_CLIENT_UTIL_H_
#define CHROME_APP_CLIENT_UTIL_H_

#include <windows.h>

#include "base/strings/string16.h"

namespace sandbox {
  struct SandboxInterfaceInfo;
}

string16 GetExecutablePath();

string16 GetCurrentModuleVersion();

class MainDllLoader {
 public:
  MainDllLoader();
  virtual ~MainDllLoader();

  
  
  
  
  
  int Launch(HINSTANCE instance, sandbox::SandboxInterfaceInfo* sbox_info);

  
  
  void RelaunchChromeBrowserWithNewCommandLineIfNeeded();

  
  
  
  virtual void OnBeforeLaunch(const string16& dll_path) {}

  
  
  
  virtual int OnBeforeExit(int return_code, const string16& dll_path) {
    return return_code;
  }

 protected:
  
  
  virtual string16 GetRegistryPath() = 0;

  HMODULE Load(string16* out_version, string16* out_file);

 private:
  
  HMODULE dll_;
};

MainDllLoader* MakeMainDllLoader();

#endif  
