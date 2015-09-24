// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_RENDERER_MEDIA_WEBRTC_LOG_MESSAGE_DELEGATE_H_
#define CONTENT_PUBLIC_RENDERER_MEDIA_WEBRTC_LOG_MESSAGE_DELEGATE_H_

#include <string>

#include "content/common/content_export.h"

namespace content {

class WebRtcLogMessageDelegate {
 public:
  
  virtual void LogMessage(const std::string& message) = 0;

 protected:
  virtual ~WebRtcLogMessageDelegate() {}
};

CONTENT_EXPORT void InitWebRtcLoggingDelegate(
    WebRtcLogMessageDelegate* delegate);

CONTENT_EXPORT void InitWebRtcLogging();

}  

#endif  
