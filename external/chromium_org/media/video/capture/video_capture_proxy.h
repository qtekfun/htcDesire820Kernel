// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_VIDEO_CAPTURE_VIDEO_CAPTURE_PROXY_H_
#define MEDIA_VIDEO_CAPTURE_VIDEO_CAPTURE_PROXY_H_

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "media/video/capture/video_capture.h"

namespace base {
class MessageLoopProxy;
}

namespace media {

class MEDIA_EXPORT VideoCaptureHandlerProxy
    : public VideoCapture::EventHandler {
 public:
  struct VideoCaptureState {
    VideoCaptureState() : started(false), frame_rate(0) {}
    bool started;
    int frame_rate;
  };

  
  VideoCaptureHandlerProxy(
      VideoCapture::EventHandler* proxied,
      scoped_refptr<base::MessageLoopProxy> main_message_loop);
  virtual ~VideoCaptureHandlerProxy();

  
  const VideoCaptureState& state() const { return state_; }

  
  virtual void OnStarted(VideoCapture* capture) OVERRIDE;
  virtual void OnStopped(VideoCapture* capture) OVERRIDE;
  virtual void OnPaused(VideoCapture* capture) OVERRIDE;
  virtual void OnError(VideoCapture* capture, int error_code) OVERRIDE;
  virtual void OnRemoved(VideoCapture* capture) OVERRIDE;
  virtual void OnFrameReady(VideoCapture* capture,
                            const scoped_refptr<VideoFrame>& frame) OVERRIDE;

 private:
  
  void OnStartedOnMainThread(
      VideoCapture* capture,
      const VideoCaptureState& state);
  void OnStoppedOnMainThread(
      VideoCapture* capture,
      const VideoCaptureState& state);
  void OnPausedOnMainThread(
      VideoCapture* capture,
      const VideoCaptureState& state);
  void OnErrorOnMainThread(
      VideoCapture* capture,
      const VideoCaptureState& state,
      int error_code);
  void OnRemovedOnMainThread(
      VideoCapture* capture,
      const VideoCaptureState& state);
  void OnFrameReadyOnMainThread(VideoCapture* capture,
                                const VideoCaptureState& state,
                                const scoped_refptr<VideoFrame>& frame);

  
  VideoCapture::EventHandler* proxied_;
  VideoCaptureState state_;

  scoped_refptr<base::MessageLoopProxy> main_message_loop_;
};

}  

#endif  
