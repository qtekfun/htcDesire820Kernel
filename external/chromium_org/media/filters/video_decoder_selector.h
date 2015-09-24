// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FILTERS_VIDEO_DECODER_SELECTOR_H_
#define MEDIA_FILTERS_VIDEO_DECODER_SELECTOR_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "media/base/decryptor.h"
#include "media/base/demuxer_stream.h"
#include "media/base/video_decoder.h"

namespace base {
class MessageLoopProxy;
}

namespace media {

class DecoderBuffer;
class DecryptingDemuxerStream;
class Decryptor;

class MEDIA_EXPORT VideoDecoderSelector {
 public:
  
  
  
  
  
  
  
  
  
  typedef base::Callback<
      void(scoped_ptr<VideoDecoder>,
           scoped_ptr<DecryptingDemuxerStream>)> SelectDecoderCB;

  
  
  
  
  VideoDecoderSelector(
      const scoped_refptr<base::MessageLoopProxy>& message_loop,
      ScopedVector<VideoDecoder> decoders,
      const SetDecryptorReadyCB& set_decryptor_ready_cb);
  ~VideoDecoderSelector();

  
  
  
  void SelectVideoDecoder(DemuxerStream* stream,
                          const SelectDecoderCB& select_decoder_cb);

  
  
  void Abort();

 private:
  void DecryptingVideoDecoderInitDone(PipelineStatus status);
  void DecryptingDemuxerStreamInitDone(PipelineStatus status);
  void InitializeDecoder();
  void DecoderInitDone(PipelineStatus status);
  void ReturnNullDecoder();

  scoped_refptr<base::MessageLoopProxy> message_loop_;
  ScopedVector<VideoDecoder> decoders_;
  SetDecryptorReadyCB set_decryptor_ready_cb_;

  DemuxerStream* input_stream_;
  SelectDecoderCB select_decoder_cb_;

  scoped_ptr<VideoDecoder> video_decoder_;
  scoped_ptr<DecryptingDemuxerStream> decrypted_stream_;

  base::WeakPtrFactory<VideoDecoderSelector> weak_ptr_factory_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(VideoDecoderSelector);
};

}  

#endif  
