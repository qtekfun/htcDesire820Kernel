// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_MEDIA_WEB_CONTENTS_VIDEO_CAPTURE_DEVICE_H_
#define CONTENT_BROWSER_RENDERER_HOST_MEDIA_WEB_CONTENTS_VIDEO_CAPTURE_DEVICE_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"
#include "media/video/capture/video_capture_device.h"

namespace content {

class VideoCaptureDeviceImpl;

class CONTENT_EXPORT WebContentsVideoCaptureDevice
    : public media::VideoCaptureDevice {
 public:
  
  
  
  
  
  
  
  
  static media::VideoCaptureDevice* Create(const std::string& device_id);

  virtual ~WebContentsVideoCaptureDevice();

  
  virtual void AllocateAndStart(const media::VideoCaptureParams& params,
                                scoped_ptr<Client> client) OVERRIDE;
  virtual void StopAndDeAllocate() OVERRIDE;

 private:
  WebContentsVideoCaptureDevice(int render_process_id, int render_view_id);

  const scoped_ptr<VideoCaptureDeviceImpl> impl_;

  DISALLOW_COPY_AND_ASSIGN(WebContentsVideoCaptureDevice);
};


}  

#endif  
