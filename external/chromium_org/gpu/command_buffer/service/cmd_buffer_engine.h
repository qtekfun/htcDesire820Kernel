// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef GPU_COMMAND_BUFFER_SERVICE_CMD_BUFFER_ENGINE_H_
#define GPU_COMMAND_BUFFER_SERVICE_CMD_BUFFER_ENGINE_H_

#include "base/basictypes.h"
#include "gpu/command_buffer/common/buffer.h"

namespace gpu {

class CommandBufferEngine {
 public:
  CommandBufferEngine() {
  }

  virtual ~CommandBufferEngine() {
  }

  
  
  
  virtual Buffer GetSharedMemoryBuffer(int32 shm_id) = 0;

  
  virtual void set_token(int32 token) = 0;

  
  virtual bool SetGetBuffer(int32 transfer_buffer_id) = 0;

  
  virtual bool SetGetOffset(int32 offset) = 0;

  
  virtual int32 GetGetOffset() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(CommandBufferEngine);
};

}  

#endif  
