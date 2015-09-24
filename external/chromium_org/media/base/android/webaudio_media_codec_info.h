// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_ANDROID_WEBAUDIO_MEDIA_CODEC_INFO_H_
#define MEDIA_BASE_ANDROID_WEBAUDIO_MEDIA_CODEC_INFO_H_

namespace media {

struct WebAudioMediaCodecInfo {
  unsigned long channel_count;
  unsigned long sample_rate;
  unsigned long number_of_frames;
};

}  
#endif 
