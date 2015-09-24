// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_CORE_H_
#define PPAPI_CPP_CORE_H_

#include "ppapi/c/ppb_core.h"


namespace pp {

class CompletionCallback;
class Module;

class Core {
 public:
  
  
  
  

  
  
  
  
  
  void AddRefResource(PP_Resource resource) {
    interface_->AddRefResource(resource);
  }

  
  
  
  
  
  
  void ReleaseResource(PP_Resource resource) {
    interface_->ReleaseResource(resource);
  }

  
  
  
  
  
  PP_Time GetTime() {
    return interface_->GetTime();
  }

  
  
  
  
  
  
  
  
  
  PP_TimeTicks GetTimeTicks() {
    return interface_->GetTimeTicks();
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void CallOnMainThread(int32_t delay_in_milliseconds,
                        const CompletionCallback& callback,
                        int32_t result = 0);


  
  
  
  
  
  
  
  
  bool IsMainThread();

 private:
  
  friend class Module;

  
  Core(const PPB_Core* inter) : interface_(inter) {}

  
  Core(const Core& other);
  Core& operator=(const Core& other);

  const PPB_Core* interface_;
};

}  

#endif  
