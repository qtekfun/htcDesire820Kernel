// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_VIDEO_CAPTURE_LINUX_VIDEO_CAPTURE_DEVICE_LINUX_H_
#define MEDIA_VIDEO_CAPTURE_LINUX_VIDEO_CAPTURE_DEVICE_LINUX_H_

#include <string>

#include "base/threading/thread.h"
#include "media/video/capture/video_capture_device.h"
#include "media/video/capture/video_capture_types.h"

namespace media {

class VideoCaptureDeviceLinux : public VideoCaptureDevice {
 public:
  explicit VideoCaptureDeviceLinux(const Name& device_name);
  virtual ~VideoCaptureDeviceLinux();

  
  virtual void AllocateAndStart(const VideoCaptureParams& params,
                                scoped_ptr<Client> client) OVERRIDE;

  virtual void StopAndDeAllocate() OVERRIDE;

 private:
  enum InternalState {
    kIdle,  
    kCapturing,  
    kError  
            
  };

  
  struct Buffer {
    Buffer() : start(0), length(0) {}
    void* start;
    size_t length;
  };

  
  void OnAllocateAndStart(int width,
                          int height,
                          int frame_rate,
                          scoped_ptr<Client> client);
  void OnStopAndDeAllocate();
  void OnCaptureTask();

  bool AllocateVideoBuffers();
  void DeAllocateVideoBuffers();
  void SetErrorState(const std::string& reason);

  InternalState state_;
  scoped_ptr<VideoCaptureDevice::Client> client_;
  Name device_name_;
  int device_fd_;  
  base::Thread v4l2_thread_;  
  Buffer* buffer_pool_;
  int buffer_pool_size_;  
  int timeout_count_;
  VideoCaptureFormat capture_format_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(VideoCaptureDeviceLinux);
};

}  

#endif  
