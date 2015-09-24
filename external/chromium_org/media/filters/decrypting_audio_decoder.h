// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FILTERS_DECRYPTING_AUDIO_DECODER_H_
#define MEDIA_FILTERS_DECRYPTING_AUDIO_DECODER_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "media/base/audio_decoder.h"
#include "media/base/decryptor.h"
#include "media/base/demuxer_stream.h"

namespace base {
class MessageLoopProxy;
}

namespace media {

class AudioTimestampHelper;
class DecoderBuffer;
class Decryptor;

class MEDIA_EXPORT DecryptingAudioDecoder : public AudioDecoder {
 public:
  
  
  
  
  
  static const int kSupportedBitsPerChannel;

  DecryptingAudioDecoder(
      const scoped_refptr<base::MessageLoopProxy>& message_loop,
      const SetDecryptorReadyCB& set_decryptor_ready_cb);
  virtual ~DecryptingAudioDecoder();

  
  virtual void Initialize(DemuxerStream* stream,
                          const PipelineStatusCB& status_cb,
                          const StatisticsCB& statistics_cb) OVERRIDE;
  virtual void Read(const ReadCB& read_cb) OVERRIDE;
  virtual void Reset(const base::Closure& closure) OVERRIDE;
  virtual int bits_per_channel() OVERRIDE;
  virtual ChannelLayout channel_layout() OVERRIDE;
  virtual int samples_per_second() OVERRIDE;

 private:
  
  
  
  
  enum State {
    kUninitialized = 0,
    kDecryptorRequested,
    kPendingDecoderInit,
    kIdle,
    kPendingConfigChange,
    kPendingDemuxerRead,
    kPendingDecode,
    kWaitingForKey,
    kDecodeFinished,
  };

  
  void SetDecryptor(Decryptor* decryptor);

  
  void FinishInitialization(bool success);

  
  void FinishConfigChange(bool success);

  
  void ReadFromDemuxerStream();
  void DecryptAndDecodeBuffer(DemuxerStream::Status status,
                              const scoped_refptr<DecoderBuffer>& buffer);

  void DecodePendingBuffer();

  
  void DeliverFrame(int buffer_size,
                    Decryptor::Status status,
                    const Decryptor::AudioBuffers& frames);

  
  
  void OnKeyAdded();

  
  void DoReset();

  
  
  void UpdateDecoderConfig();

  
  
  void EnqueueFrames(const Decryptor::AudioBuffers& frames);

  scoped_refptr<base::MessageLoopProxy> message_loop_;
  base::WeakPtrFactory<DecryptingAudioDecoder> weak_factory_;
  base::WeakPtr<DecryptingAudioDecoder> weak_this_;

  State state_;

  PipelineStatusCB init_cb_;
  StatisticsCB statistics_cb_;
  ReadCB read_cb_;
  base::Closure reset_cb_;

  
  DemuxerStream* demuxer_stream_;

  
  SetDecryptorReadyCB set_decryptor_ready_cb_;

  Decryptor* decryptor_;

  
  scoped_refptr<media::DecoderBuffer> pending_buffer_to_decode_;

  
  
  
  
  
  bool key_added_while_decode_pending_;

  Decryptor::AudioBuffers queued_audio_frames_;

  
  int bits_per_channel_;
  ChannelLayout channel_layout_;
  int samples_per_second_;

  scoped_ptr<AudioTimestampHelper> timestamp_helper_;

  DISALLOW_COPY_AND_ASSIGN(DecryptingAudioDecoder);
};

}  

#endif  
