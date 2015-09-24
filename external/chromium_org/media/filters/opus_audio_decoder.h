// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FILTERS_OPUS_AUDIO_DECODER_H_
#define MEDIA_FILTERS_OPUS_AUDIO_DECODER_H_

#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "media/base/audio_decoder.h"
#include "media/base/demuxer_stream.h"
#include "media/base/sample_format.h"

struct OpusMSDecoder;

namespace base {
class MessageLoopProxy;
}

namespace media {

class AudioBuffer;
class AudioTimestampHelper;
class DecoderBuffer;
struct QueuedAudioBuffer;

class MEDIA_EXPORT OpusAudioDecoder : public AudioDecoder {
 public:
  explicit OpusAudioDecoder(
      const scoped_refptr<base::MessageLoopProxy>& message_loop);
  virtual ~OpusAudioDecoder();

  
  virtual void Initialize(DemuxerStream* stream,
                          const PipelineStatusCB& status_cb,
                          const StatisticsCB& statistics_cb) OVERRIDE;
  virtual void Read(const ReadCB& read_cb) OVERRIDE;
  virtual int bits_per_channel() OVERRIDE;
  virtual ChannelLayout channel_layout() OVERRIDE;
  virtual int samples_per_second() OVERRIDE;
  virtual void Reset(const base::Closure& closure) OVERRIDE;

 private:
  
  void ReadFromDemuxerStream();
  void BufferReady(DemuxerStream::Status status,
                   const scoped_refptr<DecoderBuffer>& input);


  bool ConfigureDecoder();
  void CloseDecoder();
  void ResetTimestampState();
  bool Decode(const scoped_refptr<DecoderBuffer>& input,
              scoped_refptr<AudioBuffer>* output_buffer);

  scoped_refptr<base::MessageLoopProxy> message_loop_;
  base::WeakPtrFactory<OpusAudioDecoder> weak_factory_;
  base::WeakPtr<OpusAudioDecoder> weak_this_;

  DemuxerStream* demuxer_stream_;
  StatisticsCB statistics_cb_;
  OpusMSDecoder* opus_decoder_;

  
  ChannelLayout channel_layout_;
  int samples_per_second_;
  const SampleFormat sample_format_;
  const int bits_per_channel_;

  
  scoped_ptr<AudioTimestampHelper> output_timestamp_helper_;
  base::TimeDelta last_input_timestamp_;

  ReadCB read_cb_;

  
  
  
  
  
  int frames_to_discard_;

  
  
  
  int frame_delay_at_start_;
  base::TimeDelta start_input_timestamp_;

  
  
  base::TimeDelta timestamp_offset_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(OpusAudioDecoder);
};

}  

#endif  
