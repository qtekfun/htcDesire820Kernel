// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_CAMERA_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_CAMERA_H_
#pragma once

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread.h"
#include "third_party/skia/include/core/SkBitmap.h"

class Task;
namespace base {
class Thread;
}  

namespace chromeos {

class Camera : public base::RefCountedThreadSafe<Camera> {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual void OnInitializeSuccess() = 0;
    virtual void OnInitializeFailure() = 0;

    
    
    virtual void OnStartCapturingSuccess() = 0;
    virtual void OnStartCapturingFailure() = 0;

    
    
    virtual void OnCaptureSuccess() = 0;

    
    virtual void OnCaptureFailure() = 0;
  };

  
  
  
  
  Camera(Delegate* delegate, base::Thread* thread, bool mirrored);

  
  
  
  
  void Initialize(int desired_width, int desired_height);

  
  
  void Uninitialize();

  
  
  void StartCapturing();

  
  
  void StopCapturing();

  
  
  void set_delegate(Delegate* delegate) { delegate_ = delegate; }

  
  void GetFrame(SkBitmap* frame);

 private:
  
  ~Camera();
  friend class base::RefCountedThreadSafe<Camera>;

  
  
  int OpenDevice(const char* device_name) const;

  
  
  bool InitializeReadingMode(int fd);

  
  void UnmapVideoBuffers();

  
  
  
  void OnCapture();

  
  
  bool ReadFrame();

  
  
  void ProcessImage(void* data);

  
  
  void DoInitialize(int desired_width, int desired_height);
  void DoStartCapturing();
  void DoUninitialize();
  void DoStopCapturing();

  
  
  void ReportFailure();

  
  void OnInitializeSuccess();
  void OnInitializeFailure();
  void OnStartCapturingSuccess();
  void OnStartCapturingFailure();
  void OnCaptureSuccess();
  void OnCaptureFailure();

  
  
  bool IsOnCameraThread() const;

  
  void PostCameraTask(const tracked_objects::Location& from_here,
                      Task* task);

  
  struct VideoBuffer {
    void* start;
    size_t length;
  };

  
  
  Delegate* delegate_;

  
  base::Thread* thread_;

  
  
  std::string device_name_;

  
  int device_descriptor_;

  
  std::vector<VideoBuffer> buffers_;

  
  bool is_capturing_;

  
  
  
  
  int desired_width_;
  int desired_height_;

  
  
  int frame_width_;
  int frame_height_;

  
  
  bool mirrored_;

  
  SkBitmap frame_image_;

  
  mutable base::Lock image_lock_;

  
  mutable base::Lock thread_lock_;

  DISALLOW_COPY_AND_ASSIGN(Camera);
};

}  

#endif  
