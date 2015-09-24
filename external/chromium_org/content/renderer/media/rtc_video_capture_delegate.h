// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_RTC_VIDEO_CAPTURE_DELEGATE_H_
#define CONTENT_RENDERER_MEDIA_RTC_VIDEO_CAPTURE_DELEGATE_H_

#include "base/callback.h"
#include "base/message_loop/message_loop_proxy.h"
#include "content/common/media/video_capture.h"
#include "content/renderer/media/video_capture_impl_manager.h"
#include "media/video/capture/video_capture.h"

namespace content {

class RtcVideoCaptureDelegate
    : public base::RefCountedThreadSafe<RtcVideoCaptureDelegate>,
      public media::VideoCapture::EventHandler {
 public:
  enum CaptureState {
    CAPTURE_STOPPED,  
    CAPTURE_RUNNING,  
                      
    CAPTURE_FAILED,  
  };

  typedef base::Callback<void(const scoped_refptr<media::VideoFrame>&)>
      FrameCapturedCallback;
  typedef base::Callback<void(CaptureState)> StateChangeCallback;

  RtcVideoCaptureDelegate(const media::VideoCaptureSessionId id,
                          VideoCaptureImplManager* vc_manager);

  void StartCapture(const media::VideoCaptureParams& params,
                    const FrameCapturedCallback& captured_callback,
                    const StateChangeCallback& state_callback);
  void StopCapture();

  
  
  virtual void OnStarted(media::VideoCapture* capture) OVERRIDE;
  virtual void OnStopped(media::VideoCapture* capture) OVERRIDE;
  virtual void OnPaused(media::VideoCapture* capture) OVERRIDE;
  virtual void OnError(media::VideoCapture* capture, int error_code) OVERRIDE;
  virtual void OnRemoved(media::VideoCapture* capture) OVERRIDE;
  virtual void OnFrameReady(
      media::VideoCapture* capture,
      const scoped_refptr<media::VideoFrame>& frame) OVERRIDE;

 private:
  friend class base::RefCountedThreadSafe<RtcVideoCaptureDelegate>;

  virtual ~RtcVideoCaptureDelegate();

  void OnFrameReadyOnCaptureThread(
      media::VideoCapture* capture,
      const scoped_refptr<media::VideoFrame>& frame);
  void OnErrorOnCaptureThread(media::VideoCapture* capture);
  void OnRemovedOnCaptureThread(media::VideoCapture* capture);

  
  
  media::VideoCaptureSessionId session_id_;
  
  scoped_refptr<VideoCaptureImplManager> vc_manager_;
  media::VideoCapture* capture_engine_;

  
  bool got_first_frame_;
  bool error_occured_;

  
  
  FrameCapturedCallback captured_callback_;
  
  
  StateChangeCallback state_callback_;
  
  scoped_refptr<base::MessageLoopProxy> message_loop_proxy_;
};

}  

#endif  
