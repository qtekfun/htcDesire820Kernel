// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_VTABLE_PATCH_MANAGER_H_
#define CHROME_FRAME_VTABLE_PATCH_MANAGER_H_

#include <windows.h>

#include <list>

#include "base/synchronization/lock.h"

struct FunctionStub;

namespace vtable_patch {

namespace internal {

// Returns true iff *entry was rewritten.
bool ReplaceFunctionPointer(void** entry, void* new_proc, void* curr_proc);

}  

struct MethodPatchInfo {
  int index_;
  PROC method_;
  FunctionStub* stub_;
};

HRESULT PatchInterfaceMethods(void* unknown, MethodPatchInfo* patches);

HRESULT UnpatchInterfaceMethods(MethodPatchInfo* patches);

#if 0
class DynamicPatchManager {
 public:
  explicit DynamicPatchManager(const MethodPatchInfo* patch_prototype);
  ~DynamicPatchManager();

  
  
  HRESULT PatchObject(void* unknown);

  bool UnpatchAll();

 protected:
  struct PatchedObject {
    void* vtable_;
    MethodPatchInfo patch_info_[1];

    
    
    bool operator==(const PatchedObject& that) const {
      return vtable_ == that.vtable_;
    }
  };

  typedef std::list<PatchedObject*> PatchList;
  const MethodPatchInfo* patch_prototype_;
  mutable base::Lock patch_list_lock_;
  PatchList patch_list_;
};
#endif  

}  

#define BEGIN_VTABLE_PATCHES(IFName) \
    vtable_patch::MethodPatchInfo IFName##_PatchInfo[] = {
#define VTABLE_PATCH_ENTRY(index, PatchFunction) {\
      index, \
      reinterpret_cast<PROC>(PatchFunction), \
      NULL, \
    },

#define DCHECK_IS_NOT_PATCHED(IFName) \
    for (vtable_patch::MethodPatchInfo* it = IFName##_PatchInfo; \
         it->index_ != -1; ++it) { \
      DCHECK(it->stub_ == NULL); \
    }

#define DCHECK_IS_PATCHED(IFName) \
    for (vtable_patch::MethodPatchInfo* it = IFName##_PatchInfo; \
         it->index_ != -1; ++it) { \
      DCHECK(it->stub_ != NULL); \
    }

#define IS_PATCHED(IFName) \
  (IFName##_PatchInfo[0].stub_ != NULL)

#define END_VTABLE_PATCHES() \
      -1, NULL, NULL \
    };

#endif  
