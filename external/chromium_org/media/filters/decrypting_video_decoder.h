// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FILTERS_DECRYPTING_VIDEO_DECODER_H_
#define MEDIA_FILTERS_DECRYPTING_VIDEO_DECODER_H_

#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "media/base/decryptor.h"
#include "media/base/video_decoder.h"
#include "media/base/video_decoder_config.h"

namespace base {
class MessageLoopProxy;
}

namespace media {

class DecoderBuffer;
class Decryptor;

class MEDIA_EXPORT DecryptingVideoDecoder : public VideoDecoder {
 public:
  DecryptingVideoDecoder(
      const scoped_refptr<base::MessageLoopProxy>& message_loop,
      const SetDecryptorReadyCB& set_decryptor_ready_cb);
  virtual ~DecryptingVideoDecoder();

  
  virtual void Initialize(const VideoDecoderConfig& config,
                          const PipelineStatusCB& status_cb) OVERRIDE;
  virtual void Decode(const scoped_refptr<DecoderBuffer>& buffer,
                      const DecodeCB& decode_cb) OVERRIDE;
  virtual void Reset(const base::Closure& closure) OVERRIDE;
  virtual void Stop(const base::Closure& closure) OVERRIDE;

 private:
  
  
  
  enum State {
    kUninitialized = 0,
    kDecryptorRequested,
    kPendingDecoderInit,
    kIdle,
    kPendingDecode,
    kWaitingForKey,
    kDecodeFinished,
    kStopped,
    kError
  };

  
  void SetDecryptor(Decryptor* decryptor);

  
  void FinishInitialization(bool success);

  void DecodePendingBuffer();

  
  void DeliverFrame(int buffer_size,
                    Decryptor::Status status,
                    const scoped_refptr<VideoFrame>& frame);

  
  
  void OnKeyAdded();

  
  void DoReset();

  
  void DoStop();

  scoped_refptr<base::MessageLoopProxy> message_loop_;
  base::WeakPtrFactory<DecryptingVideoDecoder> weak_factory_;
  base::WeakPtr<DecryptingVideoDecoder> weak_this_;

  State state_;

  PipelineStatusCB init_cb_;
  DecodeCB decode_cb_;
  base::Closure reset_cb_;

  VideoDecoderConfig config_;

  
  SetDecryptorReadyCB set_decryptor_ready_cb_;

  Decryptor* decryptor_;

  
  scoped_refptr<media::DecoderBuffer> pending_buffer_to_decode_;

  
  
  
  
  
  bool key_added_while_decode_pending_;

  
  
  uint32 trace_id_;

  DISALLOW_COPY_AND_ASSIGN(DecryptingVideoDecoder);
};

}  

#endif  
