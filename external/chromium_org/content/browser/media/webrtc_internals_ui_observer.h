// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_MEDIA_WEBRTC_INTERNALS_UI_OBSERVER_H_
#define CONTENT_BROWSER_MEDIA_WEBRTC_INTERNALS_UI_OBSERVER_H_

#include <string>

namespace base {
class Value;
}  

namespace content {

class WebRTCInternalsUIObserver {
 public:
  virtual ~WebRTCInternalsUIObserver() {}

  
  virtual void OnUpdate(const std::string& command,
                        const base::Value* args) = 0;
};

}  

#endif  
