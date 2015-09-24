// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_MEDIA_EXYNOS_VIDEO_DECODE_ACCELERATOR_H_
#define CONTENT_COMMON_GPU_MEDIA_EXYNOS_VIDEO_DECODE_ACCELERATOR_H_

#include <queue>
#include <vector>

#include "base/callback_forward.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/thread.h"
#include "content/common/content_export.h"
#include "content/common/gpu/media/video_decode_accelerator_impl.h"
#include "media/base/limits.h"
#include "media/base/video_decoder_config.h"
#include "media/video/picture.h"
#include "ui/gfx/size.h"
#include "ui/gl/gl_bindings.h"

namespace base {
class MessageLoopProxy;
}

namespace content {
class H264Parser;

class CONTENT_EXPORT ExynosVideoDecodeAccelerator
    : public VideoDecodeAcceleratorImpl {
 public:
  ExynosVideoDecodeAccelerator(
      EGLDisplay egl_display,
      EGLContext egl_context,
      Client* client,
      const base::WeakPtr<Client>& io_client_,
      const base::Callback<bool(void)>& make_context_current,
      const scoped_refptr<base::MessageLoopProxy>& io_message_loop_proxy);
  virtual ~ExynosVideoDecodeAccelerator();

  
  
  virtual bool Initialize(media::VideoCodecProfile profile) OVERRIDE;
  virtual void Decode(const media::BitstreamBuffer& bitstream_buffer) OVERRIDE;
  virtual void AssignPictureBuffers(
      const std::vector<media::PictureBuffer>& buffers) OVERRIDE;
  virtual void ReusePictureBuffer(int32 picture_buffer_id) OVERRIDE;
  virtual void Flush() OVERRIDE;
  virtual void Reset() OVERRIDE;
  virtual void Destroy() OVERRIDE;

  
  virtual bool CanDecodeOnIOThread() OVERRIDE;

 private:
  
  enum {
    kMfcInputBufferCount = 8,
    
    
    kMfcInputBufferMaxSize = 1024 * 1024,
    
    
    
    
    kDpbOutputBufferExtraCount = media::limits::kMaxVideoFrames + 1,
  };

  
  enum State {
    kUninitialized,      
    kInitialized,        
    kDecoding,           
    kResetting,          
    kAfterReset,         
    kChangingResolution, 
                         
    kError,              
  };

  enum BufferId {
    kFlushBufferId = -2  
  };

  
  enum PollFds {
    kPollMfc = (1 << 0),
  };

  
  
  struct BitstreamBufferRef;

  
  
  struct PictureBufferArrayRef;

  
  struct EGLSyncKHRRef;

  
  struct PictureRecord;

  
  struct MfcInputRecord {
    MfcInputRecord();
    ~MfcInputRecord();
    bool at_device;         
    void* address;          
    size_t length;          
    off_t bytes_used;       
    int32 input_id;         
  };

  
  struct MfcOutputRecord {
    MfcOutputRecord();
    ~MfcOutputRecord();
    bool at_device;         
    bool at_client;         
    int fds[2];             
    EGLImageKHR egl_image;  
    EGLSyncKHR egl_sync;    
    int32 picture_id;       
    bool cleared;           
                            
  };

  
  
  

  
  
  
  void DecodeTask(const media::BitstreamBuffer& bitstream_buffer);

  
  
  void DecodeBufferTask();
  
  bool AdvanceFrameFragment(const uint8* data, size_t size, size_t* endpos);
  
  void ScheduleDecodeBufferTaskIfNeeded();

  
  
  bool DecodeBufferInitial(const void* data, size_t size, size_t* endpos);
  bool DecodeBufferContinue(const void* data, size_t size);

  
  
  
  bool AppendToInputFrame(const void* data, size_t size);
  
  bool FlushInputFrame();

  
  
  
  void ServiceDeviceTask(bool mfc_event_pending);
  
  void EnqueueMfc();
  void DequeueMfc();
  
  void DequeueMfcEvents();
  
  bool EnqueueMfcInputRecord();
  bool EnqueueMfcOutputRecord();

  
  
  
  void ReusePictureBufferTask(int32 picture_buffer_id,
                              scoped_ptr<EGLSyncKHRRef> egl_sync_ref);

  
  
  
  void FlushTask();
  
  
  
  void NotifyFlushDoneIfNeeded();

  
  
  
  void ResetTask();
  
  
  void ResetDoneTask();

  
  void DestroyTask();

  
  bool StartDevicePoll();
  
  
  bool StopDevicePoll(bool keep_mfc_input_state);
  
  bool SetDevicePollInterrupt();
  bool ClearDevicePollInterrupt();

  void StartResolutionChangeIfNeeded();
  void FinishResolutionChange();
  void ResumeAfterResolutionChange();

  
  
  bool GetFormatInfo(struct v4l2_format* format, bool* again);
  
  bool CreateBuffersForFormat(const struct v4l2_format& format);

  
  
  

  
  void DevicePollTask(unsigned int poll_fds);

  
  
  

  
  void NotifyError(Error error);

  
  
  void SetDecoderState(State state);

  
  
  
  
  

  
  bool CreateMfcInputBuffers();
  bool CreateMfcOutputBuffers();

  
  
  

  
  void DestroyMfcInputBuffers();
  void DestroyMfcOutputBuffers();
  void ResolutionChangeDestroyBuffers();

  
  void SendPictureReady();

  
  void PictureCleared();

  
  scoped_refptr<base::MessageLoopProxy> child_message_loop_proxy_;

  
  scoped_refptr<base::MessageLoopProxy> io_message_loop_proxy_;

  
  
  
  
  
  
  base::WeakPtr<ExynosVideoDecodeAccelerator> weak_this_;

  
  
  
  base::WeakPtrFactory<Client> client_ptr_factory_;
  base::WeakPtr<Client> client_;
  
  base::WeakPtr<Client> io_client_;

  
  
  
  
  
  

  
  
  base::Thread decoder_thread_;
  
  State decoder_state_;
  
  scoped_ptr<BitstreamBufferRef> decoder_current_bitstream_buffer_;
  
  
  
  int decoder_delay_bitstream_buffer_id_;
  
  int decoder_current_input_buffer_;
  
  
  
  int decoder_decode_buffer_tasks_scheduled_;
  
  int decoder_frames_at_client_;
  
  bool decoder_flushing_;
  
  
  bool resolution_change_pending_;
  
  bool resolution_change_reset_pending_;
  
  std::queue<linked_ptr<BitstreamBufferRef> > decoder_input_queue_;
  
  
  scoped_ptr<content::H264Parser> decoder_h264_parser_;
  
  bool decoder_partial_frame_pending_;

  
  
  
  
  
  
  

  
  std::queue<int> mfc_input_ready_queue_;

  
  int mfc_fd_;

  
  bool mfc_input_streamon_;
  
  int mfc_input_buffer_queued_count_;
  
  std::vector<int> mfc_free_input_buffers_;
  
  std::vector<MfcInputRecord> mfc_input_buffer_map_;

  
  bool mfc_output_streamon_;
  
  int mfc_output_buffer_queued_count_;
  
  
  std::queue<int> mfc_free_output_buffers_;
  
  std::vector<MfcOutputRecord> mfc_output_buffer_map_;
  
  uint32 mfc_output_buffer_pixelformat_;
  
  int mfc_output_dpb_size_;

  
  std::queue<PictureRecord> pending_picture_ready_;

  
  int picture_clearing_count_;

  
  
  base::WaitableEvent pictures_assigned_;

  
  gfx::Size frame_buffer_size_;

  
  
  

  
  base::Thread device_poll_thread_;
  
  
  int device_poll_interrupt_fd_;

  
  
  

  
  base::Callback<bool(void)> make_context_current_;

  
  EGLDisplay egl_display_;
  EGLContext egl_context_;

  
  media::VideoCodecProfile video_profile_;

  DISALLOW_COPY_AND_ASSIGN(ExynosVideoDecodeAccelerator);
};

}  

#endif  
