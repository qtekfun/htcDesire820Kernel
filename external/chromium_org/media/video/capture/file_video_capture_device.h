// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_VIDEO_CAPTURE_FILE_VIDEO_CAPTURE_DEVICE_H_
#define MEDIA_VIDEO_CAPTURE_FILE_VIDEO_CAPTURE_DEVICE_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/platform_file.h"
#include "base/threading/thread.h"
#include "base/threading/thread_checker.h"
#include "media/video/capture/video_capture_device.h"

namespace media {

class MEDIA_EXPORT FileVideoCaptureDevice : public VideoCaptureDevice {
 public:
  
  
  
  static VideoCaptureDevice* Create(const Name& device_name);
  static void GetDeviceNames(Names* device_names);
  static void GetDeviceSupportedFormats(const Name& device,
                                        VideoCaptureFormats* supported_formats);

  
  virtual ~FileVideoCaptureDevice();
  virtual void AllocateAndStart(
      const VideoCaptureParams& params,
      scoped_ptr<VideoCaptureDevice::Client> client) OVERRIDE;
  virtual void StopAndDeAllocate() OVERRIDE;

 private:
  
  
  explicit FileVideoCaptureDevice(const base::FilePath& file_path);
  
  
  int CalculateFrameSize();

  
  void OnAllocateAndStart(const VideoCaptureParams& params,
                          scoped_ptr<Client> client);
  void OnStopAndDeAllocate();
  void OnCaptureTask();

  
  
  base::ThreadChecker thread_checker_;

  
  
  base::Thread capture_thread_;
  
  scoped_ptr<VideoCaptureDevice::Client> client_;
  const base::FilePath file_path_;
  base::PlatformFile file_;
  scoped_ptr<uint8[]> video_frame_;
  VideoCaptureFormat capture_format_;
  int frame_size_;
  int64 current_byte_index_;
  int64 first_frame_byte_index_;

  DISALLOW_COPY_AND_ASSIGN(FileVideoCaptureDevice);
};

}  

#endif  
