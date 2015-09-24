// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_RENDERER_MEDIA_STREAM_AUDIO_SINK_H_
#define CONTENT_PUBLIC_RENDERER_MEDIA_STREAM_AUDIO_SINK_H_

#include <vector>

#include "base/basictypes.h"
#include "content/common/content_export.h"
#include "content/public/renderer/media_stream_sink.h"

namespace blink {
class WebMediaStreamTrack;
}

namespace media {
class AudioParameters;
}

namespace content {

class CONTENT_EXPORT MediaStreamAudioSink : public MediaStreamSink {
 public:
  
  
  
  static void AddToAudioTrack(MediaStreamAudioSink* sink,
                              const blink::WebMediaStreamTrack& track);

  
  
  
  static void RemoveFromAudioTrack(MediaStreamAudioSink* sink,
                                   const blink::WebMediaStreamTrack& track);

  
  
  
  
  
  
  virtual void OnData(const int16* audio_data,
                      int sample_rate,
                      int number_of_channels,
                      int number_of_frames) = 0;

  
  
  virtual void OnSetFormat(const media::AudioParameters& params) = 0;

 protected:
  virtual ~MediaStreamAudioSink() {}
};

}  

#endif  
