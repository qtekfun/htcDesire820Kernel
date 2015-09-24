// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FILTERS_DECRYPTING_DEMUXER_STREAM_H_
#define MEDIA_FILTERS_DECRYPTING_DEMUXER_STREAM_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "media/base/audio_decoder_config.h"
#include "media/base/decryptor.h"
#include "media/base/demuxer_stream.h"
#include "media/base/pipeline_status.h"
#include "media/base/video_decoder_config.h"

namespace base {
class MessageLoopProxy;
}

namespace media {

class DecoderBuffer;

class MEDIA_EXPORT DecryptingDemuxerStream : public DemuxerStream {
 public:
  DecryptingDemuxerStream(
      const scoped_refptr<base::MessageLoopProxy>& message_loop,
      const SetDecryptorReadyCB& set_decryptor_ready_cb);
  virtual ~DecryptingDemuxerStream();

  void Initialize(DemuxerStream* stream,
                  const PipelineStatusCB& status_cb);

  
  
  
  
  void Reset(const base::Closure& closure);

  
  
  
  
  
  void Stop(const base::Closure& closure);

  
  virtual void Read(const ReadCB& read_cb) OVERRIDE;
  virtual AudioDecoderConfig audio_decoder_config() OVERRIDE;
  virtual VideoDecoderConfig video_decoder_config() OVERRIDE;
  virtual Type type() OVERRIDE;
  virtual void EnableBitstreamConverter() OVERRIDE;

 private:
  
  
  
  
  enum State {
    kUninitialized = 0,
    kDecryptorRequested,
    kIdle,
    kPendingDemuxerRead,
    kPendingDecrypt,
    kWaitingForKey,
    kStopped
  };

  
  void SetDecryptor(Decryptor* decryptor);

  
  void DecryptBuffer(DemuxerStream::Status status,
                     const scoped_refptr<DecoderBuffer>& buffer);

  void DecryptPendingBuffer();

  
  void DeliverBuffer(Decryptor::Status status,
                     const scoped_refptr<DecoderBuffer>& decrypted_buffer);

  
  
  void OnKeyAdded();

  
  void DoReset();

  
  Decryptor::StreamType GetDecryptorStreamType() const;

  
  
  void InitializeDecoderConfig();

  scoped_refptr<base::MessageLoopProxy> message_loop_;
  base::WeakPtrFactory<DecryptingDemuxerStream> weak_factory_;
  base::WeakPtr<DecryptingDemuxerStream> weak_this_;

  State state_;

  PipelineStatusCB init_cb_;
  ReadCB read_cb_;
  base::Closure reset_cb_;

  
  DemuxerStream* demuxer_stream_;

  AudioDecoderConfig audio_config_;
  VideoDecoderConfig video_config_;

  
  SetDecryptorReadyCB set_decryptor_ready_cb_;

  Decryptor* decryptor_;

  
  scoped_refptr<media::DecoderBuffer> pending_buffer_to_decrypt_;

  
  
  
  
  bool key_added_while_decrypt_pending_;

  DISALLOW_COPY_AND_ASSIGN(DecryptingDemuxerStream);
};

}  

#endif  
