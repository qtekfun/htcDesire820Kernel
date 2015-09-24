// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_FRAME_FUNCTION_STUB_H_
#define CHROME_FRAME_FUNCTION_STUB_H_

#include <windows.h>

#include "base/basictypes.h"

#pragma pack(push)
#pragma pack(1)

struct FunctionStubAsm {
  
  
  
  
  uint16 jump_to_bypass_;        
  uintptr_t bypass_target_addr_; 
  uint8 pop_return_addr_;        
  uint16 push_;                  
  uintptr_t arg_addr_;           
  uint8 push_return_addr_;       
  uint16 jump_to_target;         
  uintptr_t target_addr_;        
};

#pragma pack(pop)


#ifndef _M_IX86
#error Only x86 supported right now.
#endif

struct FunctionStub {
 public:
  
  void BypassStub(void* new_target);

  inline bool is_bypassed() const {
    return bypass_address_ !=
        reinterpret_cast<uintptr_t>(&stub_.pop_return_addr_);
  }

  
  
  bool is_valid() const;

  inline PROC code() const {
    return reinterpret_cast<PROC>(const_cast<FunctionStubAsm*>(&stub_));
  }

  
  
  
  
  
  static FunctionStub* Create(uintptr_t extra_argument, void* dest);

  
  
  
  static FunctionStub* FromCode(void* address);

  
  
  static bool Destroy(FunctionStub* stub);

  
  uintptr_t argument() const { return argument_; }
  void set_argument(uintptr_t argument) { argument_ = argument; }

  uintptr_t bypass_address() const { return bypass_address_; }
  void set_bypass_address(uintptr_t bypass_address) {
    bypass_address_ = bypass_address;
  }

  uintptr_t destination_function() const { return destination_function_; }
  void set_destination_function(uintptr_t destination_function) {
    destination_function_ = destination_function;
  }

 protected:
  
  FunctionStub(uintptr_t extra_argument, void* dest);
  ~FunctionStub();

  void Init(FunctionStubAsm* stub);

  FunctionStubAsm stub_;

  
  HMODULE signature_;

  
  uintptr_t argument_;
  
  
  uintptr_t bypass_address_;
  
  
  uintptr_t destination_function_;
};

#endif  
