// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_RTC_VIDEO_DECODER_H_
#define CONTENT_RENDERER_MEDIA_RTC_VIDEO_DECODER_H_

#include <deque>
#include <map>
#include <set>
#include <utility>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/thread.h"
#include "content/common/content_export.h"
#include "media/base/bitstream_buffer.h"
#include "media/base/video_decoder.h"
#include "media/video/picture.h"
#include "media/video/video_decode_accelerator.h"
#include "third_party/webrtc/modules/video_coding/codecs/interface/video_codec_interface.h"

namespace base {
class MessageLoopProxy;
};

namespace media {
class DecoderBuffer;
class GpuVideoAcceleratorFactories;
}

namespace content {

class CONTENT_EXPORT RTCVideoDecoder
    : NON_EXPORTED_BASE(public webrtc::VideoDecoder),
      public media::VideoDecodeAccelerator::Client,
      public base::MessageLoop::DestructionObserver {
 public:
  virtual ~RTCVideoDecoder();

  
  
  static scoped_ptr<RTCVideoDecoder> Create(
      webrtc::VideoCodecType type,
      const scoped_refptr<media::GpuVideoAcceleratorFactories>& factories);

  
  
  virtual int32_t InitDecode(const webrtc::VideoCodec* codecSettings,
                             int32_t numberOfCores) OVERRIDE;
  
  virtual int32_t Decode(
      const webrtc::EncodedImage& inputImage,
      bool missingFrames,
      const webrtc::RTPFragmentationHeader* fragmentation,
      const webrtc::CodecSpecificInfo* codecSpecificInfo = NULL,
      int64_t renderTimeMs = -1) OVERRIDE;
  
  virtual int32_t RegisterDecodeCompleteCallback(
      webrtc::DecodedImageCallback* callback) OVERRIDE;
  
  
  virtual int32_t Release() OVERRIDE;
  
  
  virtual int32_t Reset() OVERRIDE;

  
  virtual void NotifyInitializeDone() OVERRIDE;
  virtual void ProvidePictureBuffers(uint32 count,
                                     const gfx::Size& size,
                                     uint32 texture_target) OVERRIDE;
  virtual void DismissPictureBuffer(int32 id) OVERRIDE;
  virtual void PictureReady(const media::Picture& picture) OVERRIDE;
  virtual void NotifyEndOfBitstreamBuffer(int32 id) OVERRIDE;
  virtual void NotifyFlushDone() OVERRIDE;
  virtual void NotifyResetDone() OVERRIDE;
  virtual void NotifyError(media::VideoDecodeAccelerator::Error error) OVERRIDE;

  
  
  virtual void WillDestroyCurrentMessageLoop() OVERRIDE;

 private:
  class SHMBuffer;
  
  struct BufferData {
    BufferData(int32 bitstream_buffer_id,
               uint32_t timestamp,
               int width,
               int height,
               size_t size);
    BufferData();
    ~BufferData();
    int32 bitstream_buffer_id;
    uint32_t timestamp;  
    uint32_t width;
    uint32_t height;
    size_t size;  
  };

  FRIEND_TEST_ALL_PREFIXES(RTCVideoDecoderTest, IsBufferAfterReset);
  FRIEND_TEST_ALL_PREFIXES(RTCVideoDecoderTest, IsFirstBufferAfterReset);

  
  RTCVideoDecoder(
      const scoped_refptr<media::GpuVideoAcceleratorFactories>& factories);

  void Initialize(base::WaitableEvent* waiter);

  
  void RequestBufferDecode();

  bool CanMoreDecodeWorkBeDone();

  
  
  bool IsBufferAfterReset(int32 id_buffer, int32 id_reset);

  
  
  bool IsFirstBufferAfterReset(int32 id_buffer, int32 id_reset);

  
  void SaveToDecodeBuffers_Locked(const webrtc::EncodedImage& input_image,
                                  scoped_ptr<SHMBuffer> shm_buffer,
                                  const BufferData& buffer_data);

  
  
  bool SaveToPendingBuffers_Locked(const webrtc::EncodedImage& input_image,
                                   const BufferData& buffer_data);

  
  void MovePendingBuffersToDecodeBuffers();

  scoped_refptr<media::VideoFrame> CreateVideoFrame(
      const media::Picture& picture,
      const media::PictureBuffer& pb,
      uint32_t timestamp,
      uint32_t width,
      uint32_t height,
      size_t size);

  
  void ResetInternal();

  
  void ReusePictureBuffer(int64 picture_buffer_id, uint32 sync_point);

  void DestroyTextures();
  void DestroyVDA();

  
  
  
  scoped_ptr<SHMBuffer> GetSHM_Locked(size_t min_size);

  
  void PutSHM_Locked(scoped_ptr<SHMBuffer> shm_buffer);

  
  void CreateSHM(int number, size_t min_size);

  
  void RecordBufferData(const BufferData& buffer_data);
  
  void GetBufferData(int32 bitstream_buffer_id,
                     uint32_t* timestamp,
                     uint32_t* width,
                     uint32_t* height,
                     size_t* size);

  
  int32_t RecordInitDecodeUMA(int32_t status);

  enum State {
    UNINITIALIZED,  
    INITIALIZED,    
    RESETTING,      
    DECODE_ERROR,   
  };

  static const int32 ID_LAST;     
  static const int32 ID_HALF;     
  static const int32 ID_INVALID;  

  
  scoped_ptr<media::VideoDecodeAccelerator> vda_;

  
  gfx::Size frame_size_;

  
  base::WeakPtr<RTCVideoDecoder> weak_this_;

  scoped_refptr<media::GpuVideoAcceleratorFactories> factories_;

  
  scoped_refptr<base::MessageLoopProxy> vda_loop_proxy_;

  
  uint32 decoder_texture_target_;

  
  std::list<BufferData> input_buffer_data_;

  
  
  std::map<int32, SHMBuffer*> bitstream_buffers_in_decoder_;

  
  std::map<int32, media::PictureBuffer> assigned_picture_buffers_;

  
  std::map<int32, media::PictureBuffer> dismissed_picture_buffers_;

  
  
  
  std::set<int32> picture_buffers_at_display_;

  
  int32 next_picture_buffer_id_;

  
  
  
  base::Lock lock_;

  
  State state_;

  
  webrtc::DecodedImageCallback* decode_complete_callback_;

  
  int num_shm_buffers_;

  
  
  
  
  std::vector<SHMBuffer*> available_shm_segments_;

  
  
  std::deque<std::pair<webrtc::EncodedImage, BufferData> > pending_buffers_;

  
  
  std::deque<std::pair<SHMBuffer*, BufferData> > decode_buffers_;

  
  int32 next_bitstream_buffer_id_;

  
  
  int32 reset_bitstream_buffer_id_;

  
  
  base::WeakPtrFactory<RTCVideoDecoder> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(RTCVideoDecoder);
};

}  

#endif  
