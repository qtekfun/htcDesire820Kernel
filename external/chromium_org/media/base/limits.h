// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_BASE_LIMITS_H_
#define MEDIA_BASE_LIMITS_H_

#include "base/basictypes.h"

namespace media {

namespace limits {

enum {
  
  kMaxDimension = (1 << 15) - 1,  

  
  kMaxCanvas = (1 << (14 * 2)),  

  
  kMaxVideoFrames = 4,

  
  
  
  
  
  
  
  
  kMaxSampleRate = 192000,
  kMinSampleRate = 3000,
  kMaxChannels = 32,
  kMaxBytesPerSample = 4,
  kMaxBitsPerSample = kMaxBytesPerSample * 8,
  kMaxSamplesPerPacket = kMaxSampleRate,
  kMaxPacketSizeInBytes =
      kMaxBytesPerSample * kMaxChannels * kMaxSamplesPerPacket,

  
  kMaxFramesPerSecond = 1000,
};

}  

}  

#endif  
