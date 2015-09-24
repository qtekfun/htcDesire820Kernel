// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_RENDERER_MEDIA_STREAM_VIDEO_SINK_H_
#define CONTENT_PUBLIC_RENDERER_MEDIA_STREAM_VIDEO_SINK_H_

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"
#include "content/public/renderer/media_stream_sink.h"

namespace media {
class VideoFrame;
}

namespace blink {
class WebMediaStreamTrack;
}

namespace content {

class CONTENT_EXPORT MediaStreamVideoSink : public MediaStreamSink {
 public:
  
  
  
  
  static void AddToVideoTrack(MediaStreamVideoSink* sink,
                              const blink::WebMediaStreamTrack& track);
  static void RemoveFromVideoTrack(MediaStreamVideoSink* sink,
                                   const blink::WebMediaStreamTrack& track);

  virtual void OnVideoFrame(const scoped_refptr<media::VideoFrame>& frame) = 0;

 protected:
  virtual ~MediaStreamVideoSink() {}
};


}  

#endif  
