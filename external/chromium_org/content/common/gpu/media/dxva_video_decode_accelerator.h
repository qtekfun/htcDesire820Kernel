// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_MEDIA_DXVA_VIDEO_DECODE_ACCELERATOR_H_
#define CONTENT_COMMON_GPU_MEDIA_DXVA_VIDEO_DECODE_ACCELERATOR_H_

#include <d3d9.h>
#include <dxva2api.h>
#include <list>
#include <map>
#include <mfidl.h>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/linked_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/win/scoped_comptr.h"
#include "content/common/content_export.h"
#include "content/common/gpu/media/video_decode_accelerator_impl.h"
#include "media/video/video_decode_accelerator.h"

interface IMFSample;
interface IDirect3DSurface9;

namespace content {

class CONTENT_EXPORT DXVAVideoDecodeAccelerator
    : public VideoDecodeAcceleratorImpl,
      NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  enum State {
    kUninitialized,   
    kNormal,          
    kResetting,       
    kStopped,         
    kFlushing,        
  };

  
  explicit DXVAVideoDecodeAccelerator(
      media::VideoDecodeAccelerator::Client* client,
      const base::Callback<bool(void)>& make_context_current);
  virtual ~DXVAVideoDecodeAccelerator();

  
  virtual bool Initialize(media::VideoCodecProfile profile) OVERRIDE;
  virtual void Decode(const media::BitstreamBuffer& bitstream_buffer) OVERRIDE;
  virtual void AssignPictureBuffers(
      const std::vector<media::PictureBuffer>& buffers) OVERRIDE;
  virtual void ReusePictureBuffer(int32 picture_buffer_id) OVERRIDE;
  virtual void Flush() OVERRIDE;
  virtual void Reset() OVERRIDE;
  virtual void Destroy() OVERRIDE;

 private:
  typedef void* EGLConfig;
  typedef void* EGLSurface;
  
  
  
  bool CreateD3DDevManager();

  
  bool InitDecoder(media::VideoCodecProfile profile);

  
  bool CheckDecoderDxvaSupport();

  
  
  bool GetStreamsInfoAndBufferReqs();

  
  
  bool SetDecoderMediaTypes();

  
  bool SetDecoderInputMediaType();

  
  bool SetDecoderOutputMediaType(const GUID& subtype);

  
  
  bool SendMFTMessage(MFT_MESSAGE_TYPE msg, int32 param);

  
  
  void DoDecode();

  
  
  
  bool ProcessOutputSample(IMFSample* sample);

  
  
  void ProcessPendingSamples();

  
  void StopOnError(media::VideoDecodeAccelerator::Error error);

  
  
  void Invalidate();

  
  
  void NotifyInputBufferRead(int input_buffer_id);

  
  void NotifyInitializeDone();

  
  void NotifyFlushDone();

  
  void NotifyResetDone();

  
  void RequestPictureBuffers(int width, int height);

  
  void NotifyPictureReady(const media::Picture& picture);

  
  
  void NotifyInputBuffersDropped();

  
  void DecodePendingInputBuffers();

  
  void FlushInternal();

  
  void DecodeInternal(const base::win::ScopedComPtr<IMFSample>& input_sample);

  
  void HandleResolutionChanged(int width, int height);

  struct DXVAPictureBuffer;
  typedef std::map<int32, linked_ptr<DXVAPictureBuffer> > OutputBuffers;

  
  void DismissStaleBuffers(const OutputBuffers& picture_buffers);

  
  media::VideoDecodeAccelerator::Client* client_;

  base::win::ScopedComPtr<IMFTransform> decoder_;

  base::win::ScopedComPtr<IDirect3D9Ex> d3d9_;
  base::win::ScopedComPtr<IDirect3DDevice9Ex> device_;
  base::win::ScopedComPtr<IDirect3DDeviceManager9> device_manager_;
  base::win::ScopedComPtr<IDirect3DQuery9> query_;
  
  
  
  
  
  uint32 dev_manager_reset_token_;

  
  EGLConfig egl_config_;

  
  State state_;

  MFT_INPUT_STREAM_INFO input_stream_info_;
  MFT_OUTPUT_STREAM_INFO output_stream_info_;

  
  struct PendingSampleInfo {
    PendingSampleInfo(int32 buffer_id, IMFSample* sample);
    ~PendingSampleInfo();

    int32 input_buffer_id;
    base::win::ScopedComPtr<IMFSample> output_sample;
  };

  typedef std::list<PendingSampleInfo> PendingOutputSamples;

  
  PendingOutputSamples pending_output_samples_;

  
  
  OutputBuffers output_picture_buffers_;

  
  bool pictures_requested_;

  
  
  int inputs_before_decode_;

  
  typedef std::list<base::win::ScopedComPtr<IMFSample>> PendingInputs;
  PendingInputs pending_input_buffers_;

  
  base::Callback<bool(void)> make_context_current_;
};

}  

#endif  
