// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_CHANNEL_MIXER_H_
#define MEDIA_BASE_CHANNEL_MIXER_H_

#include <vector>

#include "base/basictypes.h"
#include "media/base/channel_layout.h"
#include "media/base/media_export.h"

namespace media {

class AudioBus;
class AudioParameters;

class MEDIA_EXPORT ChannelMixer {
 public:
  ChannelMixer(ChannelLayout input_layout, ChannelLayout output_layout);
  ChannelMixer(const AudioParameters& input, const AudioParameters& output);
  ~ChannelMixer();

  
  void Transform(const AudioBus* input, AudioBus* output);

 private:
  void Initialize(ChannelLayout input_layout, int input_channels,
                  ChannelLayout output_layout, int output_channels);

  
  std::vector< std::vector<float> > matrix_;

  
  
  bool remapping_;

  DISALLOW_COPY_AND_ASSIGN(ChannelMixer);
};

}  

#endif  
