// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_INSTANCE_HANDLE_H_
#define PPAPI_CPP_INSTANCE_HANDLE_H_

#include "ppapi/c/pp_instance.h"


namespace pp {

class Instance;

class InstanceHandle {
 public:
  
  
  
  
  
  InstanceHandle(Instance* instance);

  
  
  
  
  
  
  
  
  explicit InstanceHandle(PP_Instance pp_instance)
      : pp_instance_(pp_instance) {}

  
  
  
  PP_Instance pp_instance() const { return pp_instance_; }

 private:
  PP_Instance pp_instance_;
};

}  

#endif  
