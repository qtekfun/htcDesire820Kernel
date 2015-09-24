// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_AUDIO_BUS_H_
#define MEDIA_BASE_AUDIO_BUS_H_

#include <vector>

#include "base/memory/aligned_memory.h"
#include "base/memory/scoped_ptr.h"
#include "media/base/media_export.h"

namespace media {
class AudioParameters;

class MEDIA_EXPORT AudioBus {
 public:
  
  
  enum { kChannelAlignment = 16 };

  
  
  static scoped_ptr<AudioBus> Create(int channels, int frames);
  static scoped_ptr<AudioBus> Create(const AudioParameters& params);

  
  
  
  static scoped_ptr<AudioBus> CreateWrapper(int channels);

  
  
  
  static scoped_ptr<AudioBus> WrapVector(
      int frames, const std::vector<float*>& channel_data);

  
  
  
  static scoped_ptr<AudioBus> WrapMemory(int channels, int frames, void* data);
  static scoped_ptr<AudioBus> WrapMemory(const AudioParameters& params,
                                         void* data);
  static int CalculateMemorySize(const AudioParameters& params);

  
  
  static int CalculateMemorySize(int channels, int frames);

  
  
  
  
  
  
  void FromInterleaved(const void* source, int frames, int bytes_per_sample);
  void ToInterleaved(int frames, int bytes_per_sample, void* dest) const;
  void ToInterleavedPartial(int start_frame, int frames, int bytes_per_sample,
                            void* dest) const;

  
  
  
  
  void FromInterleavedPartial(const void* source, int start_frame, int frames,
                              int bytes_per_sample);

  
  
  void CopyTo(AudioBus* dest) const;

  
  
  
  
  void CopyPartialFramesTo(int source_start_frame,
                           int frame_count,
                           int dest_start_frame,
                           AudioBus* dest) const;

  
  
  
  float* channel(int channel) { return channel_data_[channel]; }
  const float* channel(int channel) const { return channel_data_[channel]; }
  void SetChannelData(int channel, float* data);

  int channels() const { return static_cast<int>(channel_data_.size()); }
  int frames() const { return frames_; }
  void set_frames(int frames);

  
  void Zero();
  void ZeroFrames(int frames);
  void ZeroFramesPartial(int start_frame, int frames);

  
  
  void Scale(float volume);

 private:
  friend struct base::DefaultDeleter<AudioBus>;
  ~AudioBus();

  AudioBus(int channels, int frames);
  AudioBus(int channels, int frames, float* data);
  AudioBus(int frames, const std::vector<float*>& channel_data);
  explicit AudioBus(int channels);

  
  
  void BuildChannelData(int channels, int aligned_frame, float* data);

  
  scoped_ptr_malloc<float, base::ScopedPtrAlignedFree> data_;

  std::vector<float*> channel_data_;
  int frames_;

  
  bool can_set_channel_data_;

  DISALLOW_COPY_AND_ASSIGN(AudioBus);
};

}  

#endif  
