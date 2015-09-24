// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_VIDEO_CAPTURE_MAC_VIDEO_CAPTURE_DEVICE_MAC_H_
#define MEDIA_VIDEO_CAPTURE_MAC_VIDEO_CAPTURE_DEVICE_MAC_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop_proxy.h"
#include "media/video/capture/video_capture_device.h"
#include "media/video/capture/video_capture_types.h"

@protocol PlatformVideoCapturingMac;

namespace media {

class VideoCaptureDeviceMac : public VideoCaptureDevice {
 public:
  explicit VideoCaptureDeviceMac(const Name& device_name);
  virtual ~VideoCaptureDeviceMac();

  
  virtual void AllocateAndStart(const VideoCaptureParams& params,
                                scoped_ptr<VideoCaptureDevice::Client> client)
      OVERRIDE;
  virtual void StopAndDeAllocate() OVERRIDE;

  bool Init();

  
  void ReceiveFrame(const uint8* video_frame,
                    int video_frame_length,
                    const VideoCaptureFormat& frame_format,
                    int aspect_numerator,
                    int aspect_denominator);

  void ReceiveError(const std::string& reason);

 private:
  void SetErrorState(const std::string& reason);
  bool UpdateCaptureResolution();

  
  enum InternalState {
    kNotInitialized,
    kIdle,
    kCapturing,
    kError
  };

  Name device_name_;
  scoped_ptr<VideoCaptureDevice::Client> client_;

  VideoCaptureFormat capture_format_;
  bool sent_frame_info_;
  bool tried_to_square_pixels_;

  
  const scoped_refptr<base::MessageLoopProxy> loop_proxy_;
  InternalState state_;

  
  
  base::WeakPtrFactory<VideoCaptureDeviceMac> weak_factory_;
  base::WeakPtr<VideoCaptureDeviceMac> weak_this_;

  id<PlatformVideoCapturingMac> capture_device_;

  DISALLOW_COPY_AND_ASSIGN(VideoCaptureDeviceMac);
};

}  

#endif  
