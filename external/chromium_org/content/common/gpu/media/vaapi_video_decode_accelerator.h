// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_MEDIA_VAAPI_VIDEO_DECODE_ACCELERATOR_H_
#define CONTENT_COMMON_GPU_MEDIA_VAAPI_VIDEO_DECODE_ACCELERATOR_H_

#include <map>
#include <queue>
#include <utility>
#include <vector>

#include "base/logging.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/synchronization/condition_variable.h"
#include "base/synchronization/lock.h"
#include "base/threading/non_thread_safe.h"
#include "base/threading/thread.h"
#include "content/common/content_export.h"
#include "content/common/gpu/media/vaapi_h264_decoder.h"
#include "content/common/gpu/media/vaapi_wrapper.h"
#include "content/common/gpu/media/video_decode_accelerator_impl.h"
#include "media/base/bitstream_buffer.h"
#include "media/video/picture.h"
#include "media/video/video_decode_accelerator.h"
#include "ui/gl/gl_bindings.h"

namespace content {

class CONTENT_EXPORT VaapiVideoDecodeAccelerator
    : public VideoDecodeAcceleratorImpl {
 public:
  VaapiVideoDecodeAccelerator(
      Display* x_display, GLXContext glx_context,
      Client* client,
      const base::Callback<bool(void)>& make_context_current);
  virtual ~VaapiVideoDecodeAccelerator();

  
  virtual bool Initialize(media::VideoCodecProfile profile) OVERRIDE;
  virtual void Decode(const media::BitstreamBuffer& bitstream_buffer) OVERRIDE;
  virtual void AssignPictureBuffers(
      const std::vector<media::PictureBuffer>& buffers) OVERRIDE;
  virtual void ReusePictureBuffer(int32 picture_buffer_id) OVERRIDE;
  virtual void Flush() OVERRIDE;
  virtual void Reset() OVERRIDE;
  virtual void Destroy() OVERRIDE;

private:
  
  void NotifyPictureReady(int32 input_id, int32 output_id);

  
  void NotifyError(Error error);

  
  
  void MapAndQueueNewInputBuffer(
      const media::BitstreamBuffer& bitstream_buffer);

  
  
  
  
  bool GetInputBuffer_Locked();

  
  
  void ReturnCurrInputBuffer_Locked();

  
  
  
  
  bool FeedDecoderWithOutputSurfaces_Locked();

  
  
  
  void DecodeTask();

  
  
  
  
  void FlushTask();

  
  
  void FinishFlush();

  
  
  
  
  
  void ResetTask();

  
  
  
  void FinishReset();

  
  void Cleanup();

  
  
  bool InitializeFBConfig();

  
  
  void SurfaceReady(int32 input_id, const scoped_refptr<VASurface>& va_surface);

  
  class TFPPicture;

  
  
  
  
  void OutputPicture(const scoped_refptr<VASurface>& va_surface,
                     int32 input_id,
                     TFPPicture* tfp_picture);

  
  void TryOutputSurface();

  
  
  
  void RecycleVASurfaceID(VASurfaceID va_surface_id);

  
  
  void InitiateSurfaceSetChange(size_t num_pics, gfx::Size size);
  
  void TryFinishSurfaceSetChange();

  
  Display* x_display_;
  GLXContext glx_context_;
  base::Callback<bool(void)> make_context_current_;
  GLXFBConfig fb_config_;

  
  enum State {
    
    kUninitialized,
    
    kDecoding,
    
    kResetting,
    
    kFlushing,
    
    kIdle,
    
    kDestroying,
  };

  
  base::Lock lock_;
  State state_;

  
  struct InputBuffer {
    InputBuffer();
    ~InputBuffer();

    int32 id;
    size_t size;
    scoped_ptr<base::SharedMemory> shm;
  };

  
  typedef std::queue<linked_ptr<InputBuffer> > InputBuffers;
  InputBuffers input_buffers_;
  
  base::ConditionVariable input_ready_;

  
  linked_ptr<InputBuffer> curr_input_buffer_;

  
  typedef std::queue<int32> OutputBuffers;
  OutputBuffers output_buffers_;

  typedef std::map<int32, linked_ptr<TFPPicture> > TFPPictures;
  
  
  TFPPictures tfp_pictures_;

  
  TFPPicture* TFPPictureById(int32 picture_buffer_id);

  
  
  std::list<VASurfaceID> available_va_surfaces_;
  
  
  base::ConditionVariable surfaces_available_;

  
  
  
  
  
  
  
  
  
  typedef base::Callback<void(TFPPicture*)> OutputCB;
  std::queue<OutputCB> pending_output_cbs_;

  
  base::MessageLoop* message_loop_;

  
  
  
  
  
  
  base::WeakPtr<VaapiVideoDecodeAccelerator> weak_this_;

  
  
  base::WeakPtrFactory<Client> client_ptr_factory_;
  base::WeakPtr<Client> client_;

  scoped_ptr<VaapiWrapper> vaapi_wrapper_;

  
  
  scoped_ptr<VaapiH264Decoder> decoder_;
  base::Thread decoder_thread_;
  
  
  
  scoped_refptr<base::MessageLoopProxy> decoder_thread_proxy_;

  int num_frames_at_client_;
  int num_stream_bufs_at_decoder_;

  
  
  bool finish_flush_pending_;

  
  
  bool awaiting_va_surfaces_recycle_;

  
  size_t requested_num_pics_;
  gfx::Size requested_pic_size_;

  DISALLOW_COPY_AND_ASSIGN(VaapiVideoDecodeAccelerator);
};

}  

#endif  
