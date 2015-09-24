// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_MEDIA_MEDIA_STREAM_REQUESTER_H_
#define CONTENT_BROWSER_RENDERER_HOST_MEDIA_MEDIA_STREAM_REQUESTER_H_

#include <string>

#include "content/common/content_export.h"
#include "content/common/media/media_stream_options.h"

namespace content {

class CONTENT_EXPORT MediaStreamRequester {
 public:
  
  virtual void StreamGenerated(int render_view_id,
                               int page_request_id,
                               const std::string& label,
                               const StreamDeviceInfoArray& audio_devices,
                               const StreamDeviceInfoArray& video_devices) = 0;
  
  virtual void StreamGenerationFailed(int render_view_id,
                                      int page_request_id) = 0;
  
  
  
  virtual void DeviceStopped(int render_view_id,
                             const std::string& label,
                             const StreamDeviceInfo& device) = 0;
  
  virtual void DevicesEnumerated(int render_view_id,
                                 int page_request_id,
                                 const std::string& label,
                                 const StreamDeviceInfoArray& devices) = 0;
  
  virtual void DeviceOpened(int render_view_id,
                            int page_request_id,
                            const std::string& label,
                            const StreamDeviceInfo& device_info) = 0;

 protected:
  virtual ~MediaStreamRequester() {
  }
};

}  

#endif  
