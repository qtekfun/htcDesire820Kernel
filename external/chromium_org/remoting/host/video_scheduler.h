// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_VIDEO_SCHEDULER_H_
#define REMOTING_HOST_VIDEO_SCHEDULER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "remoting/codec/video_encoder.h"
#include "remoting/host/capture_scheduler.h"
#include "remoting/proto/video.pb.h"
#include "third_party/webrtc/modules/desktop_capture/screen_capturer.h"

namespace base {
class SingleThreadTaskRunner;
}  

namespace media {
class ScreenCapturer;
}  

namespace remoting {

class CursorShapeInfo;

namespace protocol {
class CursorShapeInfo;
class CursorShapeStub;
class VideoStub;
}  


class VideoScheduler : public base::RefCountedThreadSafe<VideoScheduler>,
                       public webrtc::DesktopCapturer::Callback,
                       public webrtc::ScreenCapturer::MouseShapeObserver {
 public:
  
  
  
  
  VideoScheduler(
      scoped_refptr<base::SingleThreadTaskRunner> capture_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> encode_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> network_task_runner,
      scoped_ptr<webrtc::ScreenCapturer> capturer,
      scoped_ptr<VideoEncoder> encoder,
      protocol::CursorShapeStub* cursor_stub,
      protocol::VideoStub* video_stub);

  
  virtual webrtc::SharedMemory* CreateSharedMemory(size_t size) OVERRIDE;
  virtual void OnCaptureCompleted(webrtc::DesktopFrame* frame) OVERRIDE;

  
  virtual void OnCursorShapeChanged(
      webrtc::MouseCursorShape* cursor_shape) OVERRIDE;

  
  void Start();

  
  
  void Stop();

  
  
  void Pause(bool pause);

  
  
  void UpdateSequenceNumber(int64 sequence_number);

 private:
  friend class base::RefCountedThreadSafe<VideoScheduler>;
  virtual ~VideoScheduler();

  

  
  void StartOnCaptureThread();

  
  void StopOnCaptureThread();

  
  void ScheduleNextCapture();

  
  void CaptureNextFrame();

  
  void FrameCaptureCompleted();

  

  
  void SendVideoPacket(scoped_ptr<VideoPacket> packet);

  
  
  void VideoFrameSentCallback();

  
  void SendCursorShape(scoped_ptr<protocol::CursorShapeInfo> cursor_shape);

  

  
  void EncodeFrame(scoped_ptr<webrtc::DesktopFrame> frame,
                   int64 sequence_number);

  void EncodedDataAvailableCallback(int64 sequence_number,
                                    scoped_ptr<VideoPacket> packet);

  
  scoped_refptr<base::SingleThreadTaskRunner> capture_task_runner_;
  scoped_refptr<base::SingleThreadTaskRunner> encode_task_runner_;
  scoped_refptr<base::SingleThreadTaskRunner> network_task_runner_;

  
  scoped_ptr<webrtc::ScreenCapturer> capturer_;

  
  scoped_ptr<VideoEncoder> encoder_;

  
  
  protocol::CursorShapeStub* cursor_stub_;
  protocol::VideoStub* video_stub_;

  
  scoped_ptr<base::OneShotTimer<VideoScheduler> > capture_timer_;

  
  
  
  int pending_frames_;

  
  bool capture_pending_;

  
  bool did_skip_frame_;

  
  bool is_paused_;

  
  int64 sequence_number_;

  
  CaptureScheduler scheduler_;

  DISALLOW_COPY_AND_ASSIGN(VideoScheduler);
};

}  

#endif  
