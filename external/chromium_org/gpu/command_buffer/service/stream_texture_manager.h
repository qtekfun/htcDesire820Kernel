// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_SERVICE_STREAM_TEXTURE_MANAGER_H_
#define GPU_COMMAND_BUFFER_SERVICE_STREAM_TEXTURE_MANAGER_H_

#include "base/basictypes.h"

namespace gpu {

class StreamTexture;

class StreamTextureManager {
 public:
  StreamTextureManager() {
  }

  virtual ~StreamTextureManager() {
  }

  
  virtual uint32 CreateStreamTexture(uint32 service_id, uint32 client_id) = 0;

  virtual void DestroyStreamTexture(uint32 service_id) = 0;

  virtual StreamTexture* LookupStreamTexture(uint32 service_id) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(StreamTextureManager);
};

}  

#endif  
