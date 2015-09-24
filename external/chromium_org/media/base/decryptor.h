// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_DECRYPTOR_H_
#define MEDIA_BASE_DECRYPTOR_H_

#include <list>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "media/base/audio_buffer.h"
#include "media/base/media_export.h"

namespace media {

class AudioDecoderConfig;
class DecoderBuffer;
class VideoDecoderConfig;
class VideoFrame;

class MEDIA_EXPORT Decryptor {
 public:
  
  
  enum Status {
    kSuccess,  
    kNoKey,  
    kNeedMoreData,  
    kError  
  };

  
  enum StreamType {
    kAudio,
    kVideo
  };

  Decryptor();
  virtual ~Decryptor();

  
  
  typedef base::Callback<void()> NewKeyCB;

  
  
  
  
  
  virtual void RegisterNewKeyCB(StreamType stream_type,
                                const NewKeyCB& key_added_cb) = 0;

  
  
  
  
  
  
  
  
  
  
  
  typedef base::Callback<void(Status,
                              const scoped_refptr<DecoderBuffer>&)> DecryptCB;

  
  
  
  
  
  
  virtual void Decrypt(StreamType stream_type,
                       const scoped_refptr<DecoderBuffer>& encrypted,
                       const DecryptCB& decrypt_cb) = 0;

  
  
  
  
  virtual void CancelDecrypt(StreamType stream_type) = 0;

  
  
  
  
  typedef base::Callback<void(bool)> DecoderInitCB;

  
  
  virtual void InitializeAudioDecoder(const AudioDecoderConfig& config,
                                      const DecoderInitCB& init_cb) = 0;
  virtual void InitializeVideoDecoder(const VideoDecoderConfig& config,
                                      const DecoderInitCB& init_cb) = 0;

  
  
  typedef std::list<scoped_refptr<AudioBuffer> > AudioBuffers;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  typedef base::Callback<void(Status, const AudioBuffers&)> AudioDecodeCB;
  typedef base::Callback<void(Status,
                              const scoped_refptr<VideoFrame>&)> VideoDecodeCB;

  
  
  
  
  
  
  
  virtual void DecryptAndDecodeAudio(
      const scoped_refptr<DecoderBuffer>& encrypted,
      const AudioDecodeCB& audio_decode_cb) = 0;
  virtual void DecryptAndDecodeVideo(
      const scoped_refptr<DecoderBuffer>& encrypted,
      const VideoDecodeCB& video_decode_cb) = 0;

  
  
  
  
  
  virtual void ResetDecoder(StreamType stream_type) = 0;

  
  
  
  
  
  
  
  
  
  virtual void DeinitializeDecoder(StreamType stream_type) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(Decryptor);
};

typedef base::Callback<void(Decryptor*)> DecryptorReadyCB;

typedef base::Callback<void(const DecryptorReadyCB&)> SetDecryptorReadyCB;

}  

#endif  
