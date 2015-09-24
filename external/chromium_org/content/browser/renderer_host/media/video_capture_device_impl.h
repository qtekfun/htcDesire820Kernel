// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_VIDEO_CAPTURE_DEVICE_IMPL_H_
#define CONTENT_BROWSER_RENDERER_HOST_VIDEO_CAPTURE_DEVICE_IMPL_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread.h"
#include "base/threading/thread_checker.h"
#include "content/browser/renderer_host/media/video_capture_oracle.h"
#include "content/common/content_export.h"
#include "media/video/capture/video_capture_device.h"

namespace content {

const int kMinFrameWidth = 2;
const int kMinFrameHeight = 2;

template<typename IntType>
IntType MakeEven(IntType x) {
  return x & static_cast<IntType>(-2);
}

#if (defined(OS_WIN) || defined(OS_MACOSX)) || defined(USE_AURA)
const bool kAcceleratedSubscriberIsSupported = true;
#else
const bool kAcceleratedSubscriberIsSupported = false;
#endif

class VideoCaptureMachine;

class ThreadSafeCaptureOracle
    : public base::RefCountedThreadSafe<ThreadSafeCaptureOracle> {
 public:
  ThreadSafeCaptureOracle(scoped_ptr<media::VideoCaptureDevice::Client> client,
                          scoped_ptr<VideoCaptureOracle> oracle,
                          const media::VideoCaptureParams& params);

  
  
  
  
  typedef base::Callback<void(base::Time timestamp, bool success)>
      CaptureFrameCallback;

  bool ObserveEventAndDecideCapture(VideoCaptureOracle::Event event,
                                    base::Time event_time,
                                    scoped_refptr<media::VideoFrame>* storage,
                                    CaptureFrameCallback* callback);

  base::TimeDelta capture_period() const {
    return oracle_->capture_period();
  }

  
  
  void UpdateCaptureSize(const gfx::Size& source_size);

  
  void Stop();

  
  void ReportError();

 private:
  friend class base::RefCountedThreadSafe<ThreadSafeCaptureOracle>;
  virtual ~ThreadSafeCaptureOracle();

  
  void DidCaptureFrame(
      scoped_refptr<media::VideoCaptureDevice::Client::Buffer> buffer,
      int frame_number,
      base::Time timestamp,
      bool success);
  
  base::Lock lock_;

  
  scoped_ptr<media::VideoCaptureDevice::Client> client_;

  
  const scoped_ptr<VideoCaptureOracle> oracle_;

  
  const media::VideoCaptureParams params_;

  
  bool capture_size_updated_;

  
  gfx::Size capture_size_;
  int frame_rate_;
};

class VideoCaptureMachine {
 public:
  VideoCaptureMachine() : started_(false) {}
  virtual ~VideoCaptureMachine() {}

  
  bool started() const { return started_; }

  
  
  virtual bool Start(
      const scoped_refptr<ThreadSafeCaptureOracle>& oracle_proxy) = 0;

  
  virtual void Stop() = 0;

 protected:
  bool started_;

 private:
  DISALLOW_COPY_AND_ASSIGN(VideoCaptureMachine);
};

class CONTENT_EXPORT VideoCaptureDeviceImpl
    : public base::SupportsWeakPtr<VideoCaptureDeviceImpl> {
 public:
  VideoCaptureDeviceImpl(scoped_ptr<VideoCaptureMachine> capture_machine);
  virtual ~VideoCaptureDeviceImpl();

  
  void AllocateAndStart(const media::VideoCaptureParams& params,
                        scoped_ptr<media::VideoCaptureDevice::Client> client);
  void StopAndDeAllocate();

 private:
  
  enum State {
    kIdle,
    kCapturing,
    kError
  };

  void TransitionStateTo(State next_state);

  
  
  void CaptureStarted(bool success);

  
  void Error();

  
  base::ThreadChecker thread_checker_;

  
  State state_;

  
  
  
  scoped_ptr<VideoCaptureMachine> capture_machine_;

  
  
  
  scoped_refptr<ThreadSafeCaptureOracle> oracle_proxy_;

  DISALLOW_COPY_AND_ASSIGN(VideoCaptureDeviceImpl);
};


}  

#endif  
