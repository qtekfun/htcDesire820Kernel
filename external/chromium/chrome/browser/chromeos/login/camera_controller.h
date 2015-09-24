// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_CAMERA_CONTROLLER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_CAMERA_CONTROLLER_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "base/threading/thread.h"
#include "chrome/browser/chromeos/login/camera.h"
#include "third_party/skia/include/core/SkBitmap.h"

namespace chromeos {

class CameraController: public Camera::Delegate {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual void OnCaptureSuccess() = 0;

    
    virtual void OnCaptureFailure() = 0;
  };

  explicit CameraController(Delegate* delegate);
  virtual ~CameraController();

  void set_frame_width(int width) { frame_width_ = width; }
  int frame_width() const { return frame_width_; }

  void set_frame_height(int height) { frame_height_ = height; }
  int frame_height() const { return frame_height_; }

  
  void Start();

  
  void Stop();

  
  void GetFrame(SkBitmap* frame) const;

  
  virtual void OnInitializeSuccess();
  virtual void OnInitializeFailure();
  virtual void OnStartCapturingSuccess();
  virtual void OnStartCapturingFailure();
  virtual void OnCaptureSuccess();
  virtual void OnCaptureFailure();

 private:
  
  int frame_width_;
  int frame_height_;

  
  scoped_refptr<Camera> camera_;

  
  int capture_failure_counter_;

  
  int camera_init_failure_counter_;

  
  scoped_ptr<base::Thread> camera_thread_;

  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(CameraController);
};

}  

#endif  

