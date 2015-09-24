// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_SOUNDS_WAV_AUDIO_HANDLER_H_
#define MEDIA_AUDIO_SOUNDS_WAV_AUDIO_HANDLER_H_

#include "base/strings/string_piece.h"
#include "media/base/media_export.h"

namespace media {

class AudioBus;

class MEDIA_EXPORT WavAudioHandler {
 public:
  explicit WavAudioHandler(const base::StringPiece& wav_data);
  virtual ~WavAudioHandler();

  
  bool AtEnd(size_t cursor) const;

  
  // the case of success stores the number of written bytes in
  // |bytes_written|. |bytes_written| should not be NULL.
  bool CopyTo(AudioBus* bus, size_t cursor, size_t* bytes_written) const;

  int size() const { return data_.size(); }
  uint16 num_channels() const { return num_channels_; }
  uint32 sample_rate() const { return sample_rate_; }
  uint32 byte_rate() const { return byte_rate_; }
  uint16 bits_per_sample() const { return bits_per_sample_; }

 private:
  
  int ParseSubChunk(const base::StringPiece& data);

  
  bool ParseFmtChunk(const base::StringPiece& data);

  
  bool ParseDataChunk(const base::StringPiece& data);

  
  base::StringPiece data_;

  uint16 num_channels_;
  uint32 sample_rate_;
  uint32 byte_rate_;
  uint16 bits_per_sample_;
  int bytes_per_sample_;
  int bytes_per_frame_;
};

}  

#endif  
