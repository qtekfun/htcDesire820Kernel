// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_IPC_VIDEO_FRAME_CAPTURER_H_
#define REMOTING_HOST_IPC_VIDEO_FRAME_CAPTURER_H_

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "third_party/webrtc/modules/desktop_capture/screen_capturer.h"

namespace IPC {
class Message;
}  

namespace media {
struct MouseCursorShape;
}  

namespace remoting {

class DesktopSessionProxy;

class IpcVideoFrameCapturer : public webrtc::ScreenCapturer {
 public:
  explicit IpcVideoFrameCapturer(
      scoped_refptr<DesktopSessionProxy> desktop_session_proxy);
  virtual ~IpcVideoFrameCapturer();

  
  virtual void Start(Callback* callback) OVERRIDE;
  virtual void Capture(const webrtc::DesktopRegion& region) OVERRIDE;

  
  virtual void SetMouseShapeObserver(
      MouseShapeObserver* mouse_shape_observer) OVERRIDE;

  
  void OnCaptureCompleted(scoped_ptr<webrtc::DesktopFrame> frame);

  
  void OnCursorShapeChanged(scoped_ptr<webrtc::MouseCursorShape> cursor_shape);

 private:
  
  webrtc::ScreenCapturer::Callback* callback_;

  MouseShapeObserver* mouse_shape_observer_;

  
  scoped_refptr<DesktopSessionProxy> desktop_session_proxy_;

  
  bool capture_pending_;

  
  base::WeakPtrFactory<IpcVideoFrameCapturer> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(IpcVideoFrameCapturer);
};

}  

#endif  
