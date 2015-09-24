// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_PPB_VIDEO_DECODER_SHARED_H_
#define PPAPI_SHARED_IMPL_PPB_VIDEO_DECODER_SHARED_H_

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ppapi/c/dev/ppb_video_decoder_dev.h"
#include "ppapi/shared_impl/resource.h"
#include "ppapi/shared_impl/tracked_callback.h"
#include "ppapi/thunk/ppb_video_decoder_api.h"

namespace gpu {
namespace gles2 {
class GLES2Implementation;
}  
}  

namespace ppapi {

class PPAPI_SHARED_EXPORT PPB_VideoDecoder_Shared
    : public Resource,
      NON_EXPORTED_BASE(public thunk::PPB_VideoDecoder_API) {
 public:
  explicit PPB_VideoDecoder_Shared(PP_Instance instance);
  explicit PPB_VideoDecoder_Shared(const HostResource& host_resource);
  virtual ~PPB_VideoDecoder_Shared();

  
  virtual thunk::PPB_VideoDecoder_API* AsPPB_VideoDecoder_API() OVERRIDE;

  
  virtual void Destroy() OVERRIDE;

 protected:
  bool SetFlushCallback(scoped_refptr<TrackedCallback> callback);
  bool SetResetCallback(scoped_refptr<TrackedCallback> callback);
  bool SetBitstreamBufferCallback(int32 bitstream_buffer_id,
                                  scoped_refptr<TrackedCallback> callback);

  void RunFlushCallback(int32 result);
  void RunResetCallback(int32 result);
  void RunBitstreamBufferCallback(int32 bitstream_buffer_id, int32 result);

  
  void FlushCommandBuffer();

  
  void InitCommon(PP_Resource graphics_context,
                  gpu::gles2::GLES2Implementation* gles2_impl);

 private:
  
  
  typedef std::map<int32, scoped_refptr<TrackedCallback> > CallbackById;

  scoped_refptr<TrackedCallback> flush_callback_;
  scoped_refptr<TrackedCallback> reset_callback_;
  CallbackById bitstream_buffer_callbacks_;

  
  
  PP_Resource graphics_context_;

  
  
  
  
  gpu::gles2::GLES2Implementation* gles2_impl_;

  DISALLOW_COPY_AND_ASSIGN(PPB_VideoDecoder_Shared);
};

}  

#endif  
