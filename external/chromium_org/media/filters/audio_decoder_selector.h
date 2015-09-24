// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FILTERS_AUDIO_DECODER_SELECTOR_H_
#define MEDIA_FILTERS_AUDIO_DECODER_SELECTOR_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "media/base/decryptor.h"
#include "media/base/media_export.h"
#include "media/base/pipeline_status.h"

namespace base {
class MessageLoopProxy;
}

namespace media {

class AudioDecoder;
class DecoderBuffer;
class DecryptingDemuxerStream;
class DemuxerStream;

class MEDIA_EXPORT AudioDecoderSelector {
 public:
  
  
  
  
  
  
  
  
  
  typedef base::Callback<
      void(scoped_ptr<AudioDecoder>,
           scoped_ptr<DecryptingDemuxerStream>)> SelectDecoderCB;

  
  
  
  
  AudioDecoderSelector(
      const scoped_refptr<base::MessageLoopProxy>& message_loop,
      ScopedVector<AudioDecoder> decoders,
      const SetDecryptorReadyCB& set_decryptor_ready_cb);
  ~AudioDecoderSelector();

  
  
  
  void SelectAudioDecoder(DemuxerStream* stream,
                          const StatisticsCB& statistics_cb,
                          const SelectDecoderCB& select_decoder_cb);

  
  
  void Abort();

 private:
  void DecryptingAudioDecoderInitDone(PipelineStatus status);
  void DecryptingDemuxerStreamInitDone(PipelineStatus status);
  void InitializeDecoder();
  void DecoderInitDone(PipelineStatus status);
  void ReturnNullDecoder();

  scoped_refptr<base::MessageLoopProxy> message_loop_;
  ScopedVector<AudioDecoder> decoders_;
  SetDecryptorReadyCB set_decryptor_ready_cb_;

  DemuxerStream* input_stream_;
  StatisticsCB statistics_cb_;
  SelectDecoderCB select_decoder_cb_;

  scoped_ptr<AudioDecoder> audio_decoder_;
  scoped_ptr<DecryptingDemuxerStream> decrypted_stream_;

  base::WeakPtrFactory<AudioDecoderSelector> weak_ptr_factory_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(AudioDecoderSelector);
};

}  

#endif  
