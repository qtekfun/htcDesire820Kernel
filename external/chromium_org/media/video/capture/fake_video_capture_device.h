// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_VIDEO_CAPTURE_FAKE_VIDEO_CAPTURE_DEVICE_H_
#define MEDIA_VIDEO_CAPTURE_FAKE_VIDEO_CAPTURE_DEVICE_H_

#include <string>

#include "base/atomicops.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/thread.h"
#include "base/threading/thread_checker.h"
#include "media/video/capture/video_capture_device.h"

namespace media {

class MEDIA_EXPORT FakeVideoCaptureDevice : public VideoCaptureDevice {
 public:
  static VideoCaptureDevice* Create(const Name& device_name);
  virtual ~FakeVideoCaptureDevice();
  
  
  static void SetFailNextCreate();
  static void SetNumberOfFakeDevices(size_t number_of_devices);
  static size_t NumberOfFakeDevices();

  static void GetDeviceNames(Names* device_names);
  static void GetDeviceSupportedFormats(const Name& device,
                                        VideoCaptureFormats* supported_formats);

  
  virtual void AllocateAndStart(const VideoCaptureParams& params,
                                scoped_ptr<VideoCaptureDevice::Client> client)
      OVERRIDE;
  virtual void StopAndDeAllocate() OVERRIDE;

 private:
  FakeVideoCaptureDevice();

  
  void OnAllocateAndStart(const VideoCaptureParams& params,
                          scoped_ptr<Client> client);
  void OnStopAndDeAllocate();
  void OnCaptureTask();
  void Reallocate();
  void PopulateFormatRoster();

  
  
  base::ThreadChecker thread_checker_;

  base::Thread capture_thread_;
  
  scoped_ptr<VideoCaptureDevice::Client> client_;
  scoped_ptr<uint8[]> fake_frame_;
  int frame_count_;
  VideoCaptureFormat capture_format_;

  
  
  
  std::vector<VideoCaptureFormat> format_roster_;
  int format_roster_index_;

  static bool fail_next_create_;
  
  
  
  
  static base::subtle::Atomic32 number_of_devices_;

  DISALLOW_COPY_AND_ASSIGN(FakeVideoCaptureDevice);
};

}  

#endif  
