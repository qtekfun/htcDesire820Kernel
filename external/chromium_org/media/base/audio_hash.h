// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_AUDIO_HASH_H_
#define MEDIA_BASE_AUDIO_HASH_H_

#include <string>

#include "base/basictypes.h"
#include "base/strings/string_piece.h"
#include "media/base/media_export.h"

namespace media {

class AudioBus;

class MEDIA_EXPORT AudioHash {
 public:
  AudioHash();
  ~AudioHash();

  
  void Update(const AudioBus* audio_bus, int frames);

  
  std::string ToString() const;

 private:
  
  
  
  enum { kHashBuckets = 6 };
  float audio_hash_[kHashBuckets];

  
  
  uint32 sample_count_;

  DISALLOW_COPY_AND_ASSIGN(AudioHash);
};

}  

#endif  
