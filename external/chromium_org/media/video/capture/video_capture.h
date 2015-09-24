// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_VIDEO_CAPTURE_VIDEO_CAPTURE_H_
#define MEDIA_VIDEO_CAPTURE_VIDEO_CAPTURE_H_

#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "media/base/media_export.h"
#include "media/video/capture/video_capture_types.h"

namespace media {

class VideoFrame;

class MEDIA_EXPORT VideoCapture {
 public:
  
  
  
  class MEDIA_EXPORT EventHandler {
   public:
    
    virtual void OnStarted(VideoCapture* capture) = 0;

    
    virtual void OnStopped(VideoCapture* capture) = 0;

    
    virtual void OnPaused(VideoCapture* capture) = 0;

    
    virtual void OnError(VideoCapture* capture, int error_code) = 0;

    
    
    virtual void OnRemoved(VideoCapture* capture) = 0;

    
    virtual void OnFrameReady(
        VideoCapture* capture,
        const scoped_refptr<media::VideoFrame>& frame) = 0;

   protected:
    virtual ~EventHandler() {}
  };

  VideoCapture() {}

  
  
  
  virtual void StartCapture(EventHandler* handler,
                            const VideoCaptureParams& params) = 0;

  
  
  virtual void StopCapture(EventHandler* handler) = 0;

  virtual bool CaptureStarted() = 0;
  virtual int CaptureFrameRate() = 0;

 protected:
  virtual ~VideoCapture() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(VideoCapture);
};

}  

#endif  
