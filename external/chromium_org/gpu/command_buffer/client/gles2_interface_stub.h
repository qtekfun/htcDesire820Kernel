// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_CLIENT_GLES2_INTERFACE_STUB_H_
#define GPU_COMMAND_BUFFER_CLIENT_GLES2_INTERFACE_STUB_H_

#include "gpu/command_buffer/client/gles2_interface.h"

namespace gpu {
namespace gles2 {

class GLES2InterfaceStub : public GLES2Interface {
 public:
  GLES2InterfaceStub();
  virtual ~GLES2InterfaceStub();

  
  
  
  #include "gpu/command_buffer/client/gles2_interface_stub_autogen.h"
};

}  
}  

#endif  
