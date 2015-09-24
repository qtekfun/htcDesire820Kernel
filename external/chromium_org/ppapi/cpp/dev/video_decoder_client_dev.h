// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEV_VIDEO_DECODER_CLIENT_DEV_H_
#define PPAPI_CPP_DEV_VIDEO_DECODER_CLIENT_DEV_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/dev/pp_video_dev.h"
#include "ppapi/cpp/instance_handle.h"

namespace pp {

class Instance;
class VideoDecoder_Dev;

class VideoDecoderClient_Dev {
 public:
  VideoDecoderClient_Dev(Instance* instance);
  virtual ~VideoDecoderClient_Dev();

  
  virtual void ProvidePictureBuffers(PP_Resource decoder,
                                     uint32_t req_num_of_bufs,
                                     const PP_Size& dimensions,
                                     uint32_t texture_target) = 0;

  
  
  virtual void DismissPictureBuffer(PP_Resource decoder,
                                    int32_t picture_buffer_id) = 0;

  
  virtual void PictureReady(PP_Resource decoder,
                            const PP_Picture_Dev& picture) = 0;

  
  virtual void NotifyError(PP_Resource decoder,
                           PP_VideoDecodeError_Dev error) = 0;

 private:
  InstanceHandle associated_instance_;
};

}  

#endif  
