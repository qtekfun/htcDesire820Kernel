// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_MEDIA_STREAM_VIDEO_TRACK_H_
#define CONTENT_RENDERER_MEDIA_MEDIA_STREAM_VIDEO_TRACK_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_vector.h"
#include "base/threading/thread_checker.h"
#include "content/common/content_export.h"
#include "content/public/renderer/media_stream_video_sink.h"
#include "content/renderer/media/media_stream_track_extra_data.h"

namespace webrtc {
class VideoTrackInterface;
}

namespace content {

class WebRtcVideoSinkAdapter;

class CONTENT_EXPORT MediaStreamVideoTrack : public MediaStreamTrackExtraData {
 public:
  MediaStreamVideoTrack(webrtc::VideoTrackInterface* track,
                        bool is_local_track);
  virtual ~MediaStreamVideoTrack();
  void AddSink(MediaStreamVideoSink* sink);
  void RemoveSink(MediaStreamVideoSink* sink);

 private:
  
  base::ThreadChecker thread_checker_;
  
  
  
  webrtc::VideoTrackInterface* video_track_;
  ScopedVector<WebRtcVideoSinkAdapter> sinks_;

  DISALLOW_COPY_AND_ASSIGN(MediaStreamVideoTrack);
};

}  

#endif  
