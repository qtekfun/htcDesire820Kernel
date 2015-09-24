// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_TOOLS_FINDER_FINDER_H__
#define SANDBOX_TOOLS_FINDER_FINDER_H__

#include "sandbox/win/src/restricted_token_utils.h"
#include "sandbox/win/tools/finder/ntundoc.h"

enum Stats {
  READ = 0,     
  WRITE,        
  ALL,          
  PARSE,        
  BROKEN,       
  SIZE_STATS    
};

const int kScanRegistry       = 0x01;
const int kScanFileSystem     = 0x02;
const int kScanKernelObjects  = 0x04;

const int kTestForRead        = 0x01;
const int kTestForWrite       = 0x02;
const int kTestForAll         = 0x04;

#define FS_ERR L"FILE-ERROR"
#define OBJ_ERR L"OBJ-ERROR"
#define REG_ERR L"REG_ERROR"
#define OBJ L"OBJ"
#define FS L"FILE"
#define REG L"REG"

class Impersonater {
 public:
  Impersonater(HANDLE token_handle) {
    if (token_handle)
      ::ImpersonateLoggedOnUser(token_handle);
  };
  ~Impersonater() {
    ::RevertToSelf();
  };
};

class Finder {
 public:
  Finder();
  ~Finder();
  DWORD Init(sandbox::TokenLevel token_type, DWORD object_type,
             DWORD access_type, FILE *file_output);
  DWORD Scan();

 private:
  
  
  
  
  DWORD ParseFileSystem(ATL::CString path);

  
  
  
  
  DWORD ParseRegistry(HKEY key, ATL::CString print_name);

  
  
  
  
  
  DWORD ParseKernelObjects(ATL::CString path);

  
  
  DWORD TestFileAccess(ATL::CString path);

  
  
  
  
  DWORD TestRegAccess(HKEY key, ATL::CString name, ATL::CString print_name);

  
  
  
  DWORD TestKernelObjectAccess(ATL::CString path, ATL::CString type);

  
  void Output(ATL::CString type, ATL::CString access, ATL::CString info) {
    fprintf(file_output_, "\n%S;%S;%S", type.GetBuffer(), access.GetBuffer(),
            info.GetBuffer());
  };

  
  void Output(ATL::CString type, DWORD error, ATL::CString info) {
    fprintf(file_output_, "\n%S;0x%X;%S", type.GetBuffer(), error,
            info.GetBuffer());
  };

  
  
  
  
  bool GetFunctionForType(ATL::CString type, NTGENERICOPEN * func_to_call);

  
  
  
  
  DWORD InitNT();

  
  
  
  NTSTATUS NtGenericOpen(ACCESS_MASK desired_access,
                         OBJECT_ATTRIBUTES *object_attributes,
                         NTGENERICOPEN func_to_call,
                         HANDLE *handle);

  
  DWORD object_type_;
  
  DWORD access_type_;
  
  FILE * file_output_;
  
  HANDLE token_handle_;
  
  
  int filesystem_stats_[SIZE_STATS];
  int registry_stats_[SIZE_STATS];
  int kernel_object_stats_[SIZE_STATS];
};

#endif  
