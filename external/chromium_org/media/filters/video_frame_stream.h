// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FILTERS_VIDEO_FRAME_STREAM_H_
#define MEDIA_FILTERS_VIDEO_FRAME_STREAM_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "media/base/decryptor.h"
#include "media/base/demuxer_stream.h"
#include "media/base/media_export.h"
#include "media/base/pipeline_status.h"
#include "media/base/video_decoder.h"

namespace base {
class MessageLoopProxy;
}

namespace media {

class DecryptingDemuxerStream;
class VideoDecoderSelector;

class MEDIA_EXPORT VideoFrameStream {
 public:
  
  typedef base::Callback<void(bool success, bool has_alpha)> InitCB;

  enum Status {
    OK,  
    ABORTED,  
    DEMUXER_READ_ABORTED,  
    DECODE_ERROR,  
    DECRYPT_ERROR  
  };

  
  typedef base::Callback<void(Status, const scoped_refptr<VideoFrame>&)> ReadCB;

  VideoFrameStream(const scoped_refptr<base::MessageLoopProxy>& message_loop,
                   ScopedVector<VideoDecoder> decoders,
                   const SetDecryptorReadyCB& set_decryptor_ready_cb);
  virtual ~VideoFrameStream();

  
  
  void Initialize(DemuxerStream* stream,
                  const StatisticsCB& statistics_cb,
                  const InitCB& init_cb);

  
  
  
  
  void Read(const ReadCB& read_cb);

  
  
  
  
  
  void Reset(const base::Closure& closure);

  
  
  
  
  
  void Stop(const base::Closure& closure);

  
  
  bool CanReadWithoutStalling() const;

 private:
  enum State {
    STATE_UNINITIALIZED,
    STATE_INITIALIZING,
    STATE_NORMAL,  
    STATE_FLUSHING_DECODER,
    STATE_PENDING_DEMUXER_READ,
    STATE_REINITIALIZING_DECODER,
    STATE_STOPPED,
    STATE_ERROR
  };

  
  
  
  void OnDecoderSelected(
      scoped_ptr<VideoDecoder> selected_decoder,
      scoped_ptr<DecryptingDemuxerStream> decrypting_demuxer_stream);

  
  void SatisfyRead(Status status, const scoped_refptr<VideoFrame>& frame);

  
  void AbortRead();

  
  void Decode(const scoped_refptr<DecoderBuffer>& buffer);

  
  
  void FlushDecoder();

  
  void OnFrameReady(int buffer_size,
                    const VideoDecoder::Status status,
                    const scoped_refptr<VideoFrame>& frame);

  
  void ReadFromDemuxerStream();

  
  void OnBufferReady(DemuxerStream::Status status,
                     const scoped_refptr<DecoderBuffer>& buffer);

  void ReinitializeDecoder();

  
  void OnDecoderReinitialized(PipelineStatus status);

  void ResetDecoder();
  void OnDecoderReset();

  void StopDecoder();
  void OnDecoderStopped();

  scoped_refptr<base::MessageLoopProxy> message_loop_;
  base::WeakPtrFactory<VideoFrameStream> weak_factory_;

  State state_;

  StatisticsCB statistics_cb_;
  InitCB init_cb_;

  ReadCB read_cb_;
  base::Closure reset_cb_;
  base::Closure stop_cb_;

  DemuxerStream* stream_;

  scoped_ptr<VideoDecoderSelector> decoder_selector_;

  
  scoped_ptr<VideoDecoder> decoder_;
  scoped_ptr<DecryptingDemuxerStream> decrypting_demuxer_stream_;

  DISALLOW_COPY_AND_ASSIGN(VideoFrameStream);
};

}  

#endif  
