// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_VIDEO_CAPTURE_WIN_VIDEO_CAPTURE_DEVICE_MF_WIN_H_
#define MEDIA_VIDEO_CAPTURE_WIN_VIDEO_CAPTURE_DEVICE_MF_WIN_H_

#include <mfidl.h>
#include <mfreadwrite.h>

#include <vector>

#include "base/synchronization/lock.h"
#include "base/threading/non_thread_safe.h"
#include "base/win/scoped_comptr.h"
#include "media/base/media_export.h"
#include "media/video/capture/video_capture_device.h"

interface IMFSourceReader;

namespace media {

class MFReaderCallback;

class MEDIA_EXPORT VideoCaptureDeviceMFWin
    : public base::NonThreadSafe,
      public VideoCaptureDevice {
 public:
  explicit VideoCaptureDeviceMFWin(const Name& device_name);
  virtual ~VideoCaptureDeviceMFWin();

  
  
  bool Init();

  
  virtual void AllocateAndStart(const VideoCaptureParams& params,
                                scoped_ptr<VideoCaptureDevice::Client> client)
      OVERRIDE;
  virtual void StopAndDeAllocate() OVERRIDE;

  
  
  
  
  
  static bool PlatformSupported();

  static void GetDeviceNames(Names* device_names);

  
  void OnIncomingCapturedFrame(
      const uint8* data,
      int length,
      const base::Time& time_stamp,
      int rotation);

 private:
  void OnError(HRESULT hr);

  Name name_;
  base::win::ScopedComPtr<IMFActivate> device_;
  scoped_refptr<MFReaderCallback> callback_;

  base::Lock lock_;  
  scoped_ptr<VideoCaptureDevice::Client> client_;
  base::win::ScopedComPtr<IMFSourceReader> reader_;
  VideoCaptureFormat capture_format_;
  bool capture_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(VideoCaptureDeviceMFWin);
};

}  

#endif  
