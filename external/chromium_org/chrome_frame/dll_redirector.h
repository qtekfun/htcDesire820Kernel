// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_DLL_REDIRECTOR_H_
#define CHROME_FRAME_DLL_REDIRECTOR_H_

#include <ObjBase.h>
#include <windows.h>
#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/memory/singleton.h"

namespace ATL {
class CSecurityAttributes;
}

namespace base {
class Version;
}

class DllRedirector {
 public:
  
  static DllRedirector* GetInstance();

  virtual ~DllRedirector();

  
  
  
  
  bool DllRedirector::RegisterAsFirstCFModule();

  
  
  void DllRedirector::UnregisterAsFirstCFModule();

  
  
  
  
  
  
  
  LPFNGETCLASSOBJECT GetDllGetClassObjectPtr();

 protected:
  DllRedirector();
  friend struct DefaultSingletonTraits<DllRedirector>;

  
  explicit DllRedirector(const char* shared_memory_name);

  
  virtual HMODULE GetFirstModule();

  
  
  virtual base::Version* GetCurrentModuleVersion();

  
  
  
  
  virtual HMODULE LoadVersionedModule(base::Version* version);

  
  
  virtual bool BuildSecurityAttributesForLock(
      ATL::CSecurityAttributes* sec_attr);

  
  
  virtual bool SetFileMappingToReadOnly(base::SharedMemoryHandle mapping);

  
  scoped_ptr<base::SharedMemory> shared_memory_;

  
  scoped_ptr<base::Version> dll_version_;

  
  
  
  HMODULE first_module_handle_;

  
  std::string shared_memory_name_;

  friend class ModuleUtilsTest;

  DISALLOW_COPY_AND_ASSIGN(DllRedirector);
};

#endif  
