// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_RTC_VIDEO_CAPTURER_H_
#define CONTENT_RENDERER_MEDIA_RTC_VIDEO_CAPTURER_H_

#include <vector>

#include "base/compiler_specific.h"
#include "content/renderer/media/rtc_video_capture_delegate.h"
#include "third_party/libjingle/source/talk/media/base/videocapturer.h"

namespace content {
class VideoCaptureImplManager;

class RtcVideoCapturer
    : public cricket::VideoCapturer {
 public:
  RtcVideoCapturer(const media::VideoCaptureSessionId id,
                   VideoCaptureImplManager* vc_manager,
                   bool is_screencast);
  virtual ~RtcVideoCapturer();

  
  
  virtual cricket::CaptureState Start(
      const cricket::VideoFormat& capture_format) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual bool IsRunning() OVERRIDE;
  virtual bool GetPreferredFourccs(std::vector<uint32>* fourccs) OVERRIDE;
  virtual bool GetBestCaptureFormat(const cricket::VideoFormat& desired,
                                    cricket::VideoFormat* best_format) OVERRIDE;
  virtual bool IsScreencast() const OVERRIDE;

 private:
  
  virtual void OnFrameCaptured(const scoped_refptr<media::VideoFrame>& frame);

  
  void OnStateChange(RtcVideoCaptureDelegate::CaptureState state);

  const bool is_screencast_;
  scoped_refptr<RtcVideoCaptureDelegate> delegate_;
  VideoCaptureState state_;
  base::TimeDelta first_frame_timestamp_;

  DISALLOW_COPY_AND_ASSIGN(RtcVideoCapturer);
};

}  

#endif  
