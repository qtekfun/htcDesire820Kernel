// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FILTERS_GPU_VIDEO_ACCELERATOR_FACTORIES_H_
#define MEDIA_FILTERS_GPU_VIDEO_ACCELERATOR_FACTORIES_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "media/video/video_decode_accelerator.h"
#include "media/video/video_encode_accelerator.h"

namespace base {
class MessageLoopProxy;
class SharedMemory;
}

class SkBitmap;

namespace media {

class MEDIA_EXPORT GpuVideoAcceleratorFactories
    : public base::RefCountedThreadSafe<GpuVideoAcceleratorFactories> {
 public:
  
  virtual scoped_ptr<VideoDecodeAccelerator> CreateVideoDecodeAccelerator(
      VideoCodecProfile profile,
      VideoDecodeAccelerator::Client* client) = 0;

  
  virtual scoped_ptr<VideoEncodeAccelerator> CreateVideoEncodeAccelerator(
      VideoEncodeAccelerator::Client* client) = 0;

  
  virtual uint32 CreateTextures(int32 count,
                                const gfx::Size& size,
                                std::vector<uint32>* texture_ids,
                                std::vector<gpu::Mailbox>* texture_mailboxes,
                                uint32 texture_target) = 0;
  virtual void DeleteTexture(uint32 texture_id) = 0;

  virtual void WaitSyncPoint(uint32 sync_point) = 0;

  
  virtual void ReadPixels(uint32 texture_id,
                          const gfx::Size& size,
                          const SkBitmap& pixels) = 0;

  
  
  virtual base::SharedMemory* CreateSharedMemory(size_t size) = 0;

  
  virtual scoped_refptr<base::MessageLoopProxy> GetMessageLoop() = 0;

  
  
  virtual void Abort() = 0;

  
  virtual bool IsAborted() = 0;

 protected:
  friend class base::RefCountedThreadSafe<GpuVideoAcceleratorFactories>;
  virtual ~GpuVideoAcceleratorFactories();
};

}  

#endif  
