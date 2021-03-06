// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_CHANNEL_LAYOUT_H_
#define MEDIA_BASE_CHANNEL_LAYOUT_H_

#include "media/base/media_export.h"

namespace media {

enum ChannelLayout {
  CHANNEL_LAYOUT_NONE = 0,
  CHANNEL_LAYOUT_UNSUPPORTED = 1,

  
  CHANNEL_LAYOUT_MONO = 2,

  
  CHANNEL_LAYOUT_STEREO = 3,

  
  CHANNEL_LAYOUT_2_1 = 4,

  
  CHANNEL_LAYOUT_SURROUND = 5,

  
  CHANNEL_LAYOUT_4_0 = 6,

  
  CHANNEL_LAYOUT_2_2 = 7,

  
  CHANNEL_LAYOUT_QUAD = 8,

  
  CHANNEL_LAYOUT_5_0 = 9,

  
  CHANNEL_LAYOUT_5_1 = 10,

  
  CHANNEL_LAYOUT_5_0_BACK = 11,

  
  CHANNEL_LAYOUT_5_1_BACK = 12,

  
  CHANNEL_LAYOUT_7_0 = 13,

  
  CHANNEL_LAYOUT_7_1 = 14,

  
  CHANNEL_LAYOUT_7_1_WIDE = 15,

  
  CHANNEL_LAYOUT_STEREO_DOWNMIX = 16,

  
  CHANNEL_LAYOUT_2POINT1 = 17,

  
  CHANNEL_LAYOUT_3_1 = 18,

  
  CHANNEL_LAYOUT_4_1 = 19,

  
  CHANNEL_LAYOUT_6_0 = 20,

  
  CHANNEL_LAYOUT_6_0_FRONT = 21,

  
  CHANNEL_LAYOUT_HEXAGONAL = 22,

  
  CHANNEL_LAYOUT_6_1 = 23,

  
  CHANNEL_LAYOUT_6_1_BACK = 24,

  
  CHANNEL_LAYOUT_6_1_FRONT = 25,

  
  CHANNEL_LAYOUT_7_0_FRONT = 26,

  
  CHANNEL_LAYOUT_7_1_WIDE_BACK = 27,

  
  CHANNEL_LAYOUT_OCTAGONAL = 28,

  
  CHANNEL_LAYOUT_DISCRETE = 29,

  
  CHANNEL_LAYOUT_MAX  
};

enum Channels {
  LEFT = 0,
  RIGHT,
  CENTER,
  LFE,
  BACK_LEFT,
  BACK_RIGHT,
  LEFT_OF_CENTER,
  RIGHT_OF_CENTER,
  BACK_CENTER,
  SIDE_LEFT,
  SIDE_RIGHT,
  CHANNELS_MAX
};

MEDIA_EXPORT int ChannelOrder(ChannelLayout layout, Channels channel);

MEDIA_EXPORT int ChannelLayoutToChannelCount(ChannelLayout layout);

MEDIA_EXPORT ChannelLayout GuessChannelLayout(int channels);

MEDIA_EXPORT const char* ChannelLayoutToString(ChannelLayout layout);

}  

#endif  
