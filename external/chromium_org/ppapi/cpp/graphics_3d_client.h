// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_GRAPHICS_3D_CLIENT_H_
#define PPAPI_CPP_GRAPHICS_3D_CLIENT_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/cpp/instance_handle.h"


namespace pp {

class Instance;

class Graphics3DClient {
 public:
  
  
  
  
  
  explicit Graphics3DClient(Instance* instance);

  
  virtual ~Graphics3DClient();

  
  
  virtual void Graphics3DContextLost() = 0;

 private:
  InstanceHandle associated_instance_;
};

}  

#endif  
