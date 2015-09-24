// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_MEDIA_EXYNOS_VIDEO_ENCODE_ACCELERATOR_H_
#define CONTENT_COMMON_GPU_MEDIA_EXYNOS_VIDEO_ENCODE_ACCELERATOR_H_

#include <list>
#include <vector>

#include "base/memory/linked_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread.h"
#include "content/common/content_export.h"
#include "media/video/video_encode_accelerator.h"
#include "ui/gfx/size.h"

namespace base {

class MessageLoopProxy;

}  

namespace media {

class BitstreamBuffer;

}  

namespace content {

class CONTENT_EXPORT ExynosVideoEncodeAccelerator
    : public media::VideoEncodeAccelerator {
 public:
  explicit ExynosVideoEncodeAccelerator(
      media::VideoEncodeAccelerator::Client* client);
  virtual ~ExynosVideoEncodeAccelerator();

  
  virtual void Initialize(media::VideoFrame::Format format,
                          const gfx::Size& input_visible_size,
                          media::VideoCodecProfile output_profile,
                          uint32 initial_bitrate) OVERRIDE;
  virtual void Encode(const scoped_refptr<media::VideoFrame>& frame,
                      bool force_keyframe) OVERRIDE;
  virtual void UseOutputBitstreamBuffer(
      const media::BitstreamBuffer& buffer) OVERRIDE;
  virtual void RequestEncodingParametersChange(uint32 bitrate,
                                               uint32 framerate) OVERRIDE;
  virtual void Destroy() OVERRIDE;

  static std::vector<media::VideoEncodeAccelerator::SupportedProfile>
      GetSupportedProfiles();

 private:
  
  
  struct BitstreamBufferRef;

  
  struct GscInputRecord {
    GscInputRecord();
    bool at_device;
    scoped_refptr<media::VideoFrame> frame;
  };

  
  struct GscOutputRecord {
    GscOutputRecord();
    bool at_device;
    int mfc_input;
  };

  
  struct MfcInputRecord {
    MfcInputRecord();
    bool at_device;
    int fd[2];
  };

  
  struct MfcOutputRecord {
    MfcOutputRecord();
    bool at_device;
    linked_ptr<BitstreamBufferRef> buffer_ref;
    void* address;
    size_t length;
  };

  enum {
    kInitialFramerate = 30,
    
    kGscInputBufferCount = 2,
    kGscOutputBufferCount = 2,
    kMfcOutputBufferCount = 2,
    
    
    kMfcOutputBufferSize = (2 * 1024 * 1024),
  };

  
  enum State {
    kUninitialized,  
    kInitialized,    
    kEncoding,       
    kError,          
  };

  
  
  

  
  void EncodeTask(const scoped_refptr<media::VideoFrame>& frame,
                  bool force_keyframe);

  
  
  void UseOutputBitstreamBufferTask(scoped_ptr<BitstreamBufferRef> buffer_ref);

  
  void DestroyTask();

  
  
  void ServiceDeviceTask();

  
  void EnqueueGsc();
  void DequeueGsc();
  void EnqueueMfc();
  void DequeueMfc();
  
  bool EnqueueGscInputRecord();
  bool EnqueueGscOutputRecord();
  bool EnqueueMfcInputRecord();
  bool EnqueueMfcOutputRecord();

  
  bool StartDevicePoll();
  bool StopDevicePoll();
  
  bool SetDevicePollInterrupt();
  bool ClearDevicePollInterrupt();

  
  
  

  
  void DevicePollTask(unsigned int poll_fds);

  
  
  

  
  void NotifyError(Error error);

  
  
  void SetEncoderState(State state);

  
  
  
  
  

  
  void RequestEncodingParametersChangeTask(uint32 bitrate, uint32 framerate);

  
  bool CreateGscInputBuffers();
  bool CreateGscOutputBuffers();
  bool SetMfcFormats();
  bool InitMfcControls();
  bool CreateMfcInputBuffers();
  bool CreateMfcOutputBuffers();

  
  void DestroyGscInputBuffers();
  void DestroyGscOutputBuffers();
  void DestroyMfcInputBuffers();
  void DestroyMfcOutputBuffers();

  
  const scoped_refptr<base::MessageLoopProxy> child_message_loop_proxy_;

  
  
  
  
  
  
  base::WeakPtrFactory<ExynosVideoEncodeAccelerator> weak_this_ptr_factory_;
  base::WeakPtr<ExynosVideoEncodeAccelerator> weak_this_;

  
  
  
  base::WeakPtrFactory<Client> client_ptr_factory_;
  base::WeakPtr<Client> client_;

  
  
  
  
  
  

  
  
  base::Thread encoder_thread_;
  
  State encoder_state_;
  
  gfx::Size input_visible_size_;
  gfx::Size input_allocated_size_;
  
  gfx::Size converted_visible_size_;
  gfx::Size converted_allocated_size_;
  
  gfx::Size output_visible_size_;
  
  size_t output_buffer_byte_size_;

  
  
  scoped_ptr<uint8[]> stream_header_;
  size_t stream_header_size_;

  
  uint32 input_format_fourcc_;
  uint32 output_format_fourcc_;

  
  std::list<scoped_refptr<media::VideoFrame> > encoder_input_queue_;

  
  int gsc_fd_;
  
  bool gsc_input_streamon_;
  
  int gsc_input_buffer_queued_count_;
  
  std::vector<int> gsc_free_input_buffers_;
  
  std::vector<GscInputRecord> gsc_input_buffer_map_;

  
  bool gsc_output_streamon_;
  
  int gsc_output_buffer_queued_count_;
  
  std::vector<int> gsc_free_output_buffers_;
  
  std::vector<GscOutputRecord> gsc_output_buffer_map_;

  
  std::list<int> mfc_ready_input_buffers_;

  
  int mfc_fd_;

  
  bool mfc_input_streamon_;
  
  int mfc_input_buffer_queued_count_;
  
  std::vector<int> mfc_free_input_buffers_;
  
  std::vector<MfcInputRecord> mfc_input_buffer_map_;

  
  bool mfc_output_streamon_;
  
  int mfc_output_buffer_queued_count_;
  
  std::vector<int> mfc_free_output_buffers_;
  
  std::vector<MfcOutputRecord> mfc_output_buffer_map_;

  
  
  std::vector<linked_ptr<BitstreamBufferRef> > encoder_output_queue_;

  
  
  
  

  
  base::Thread device_poll_thread_;
  
  
  int device_poll_interrupt_fd_;

  DISALLOW_COPY_AND_ASSIGN(ExynosVideoEncodeAccelerator);
};

}  

#endif  
