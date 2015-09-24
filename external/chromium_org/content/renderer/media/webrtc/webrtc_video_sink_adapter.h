// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_WEBRTC_WEBRTC_VIDEO_SINK_ADAPTER_H_
#define CONTENT_RENDERER_MEDIA_WEBRTC_WEBRTC_VIDEO_SINK_ADAPTER_H_

#include "base/memory/weak_ptr.h"
#include "content/common/content_export.h"
#include "content/public/renderer/media_stream_video_sink.h"
#include "third_party/libjingle/source/talk/app/webrtc/mediastreaminterface.h"

namespace base {
class MessageLoopProxy;
}

namespace content {

class CONTENT_EXPORT WebRtcVideoSinkAdapter
    : NON_EXPORTED_BASE(public webrtc::VideoRendererInterface),
      NON_EXPORTED_BASE(public webrtc::ObserverInterface),
      public base::SupportsWeakPtr<WebRtcVideoSinkAdapter> {
 public:
  WebRtcVideoSinkAdapter(webrtc::VideoTrackInterface* video_track,
                         MediaStreamVideoSink* sink);
  virtual ~WebRtcVideoSinkAdapter();

  MediaStreamVideoSink* sink() const { return sink_; }

 protected:
  
  
  virtual void SetSize(int width, int height) OVERRIDE;
  virtual void RenderFrame(const cricket::VideoFrame* frame) OVERRIDE;

  
  
  
  virtual void OnChanged() OVERRIDE;

 private:
  void DoRenderFrameOnMainThread(scoped_refptr<media::VideoFrame> video_frame);

  scoped_refptr<base::MessageLoopProxy> message_loop_proxy_;
  MediaStreamVideoSink* sink_;
  
  scoped_refptr<webrtc::VideoTrackInterface> video_track_;
  webrtc::MediaStreamTrackInterface::TrackState state_;
  bool track_enabled_;

  DISALLOW_COPY_AND_ASSIGN(WebRtcVideoSinkAdapter);
};

}  

#endif  
