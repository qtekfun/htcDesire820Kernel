// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_MEDIA_VIDEO_CAPTURE_CONTROLLER_EVENT_HANDLER_H_
#define CONTENT_BROWSER_RENDERER_HOST_MEDIA_VIDEO_CAPTURE_CONTROLLER_EVENT_HANDLER_H_

#include "base/memory/shared_memory.h"
#include "base/time/time.h"
#include "content/common/content_export.h"
#include "media/video/capture/video_capture_types.h"

namespace content {

struct CONTENT_EXPORT VideoCaptureControllerID {
  explicit VideoCaptureControllerID(int device_id);

  bool operator<(const VideoCaptureControllerID& vc) const;
  bool operator==(const VideoCaptureControllerID& vc) const;

  int device_id;
};

class CONTENT_EXPORT VideoCaptureControllerEventHandler {
 public:
  
  virtual void OnError(const VideoCaptureControllerID& id) = 0;

  
  virtual void OnBufferCreated(const VideoCaptureControllerID& id,
                               base::SharedMemoryHandle handle,
                               int length,
                               int buffer_id) = 0;

  
  virtual void OnBufferDestroyed(const VideoCaptureControllerID& id,
                                 int buffer_id) = 0;

  
  virtual void OnBufferReady(
      const VideoCaptureControllerID& id,
      int buffer_id,
      base::Time timestamp,
      const media::VideoCaptureFormat& format) = 0;

  
  virtual void OnEnded(const VideoCaptureControllerID& id) = 0;

 protected:
  virtual ~VideoCaptureControllerEventHandler() {}
};

}  

#endif  
