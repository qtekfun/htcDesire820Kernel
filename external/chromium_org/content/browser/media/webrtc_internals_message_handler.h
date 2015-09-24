// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_MEDIA_WEBRTC_INTERNALS_MESSAGE_HANDLER_H_
#define CONTENT_BROWSER_MEDIA_WEBRTC_INTERNALS_MESSAGE_HANDLER_H_

#include "base/memory/ref_counted.h"
#include "content/browser/media/webrtc_internals_ui_observer.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace base {
class ListValue;
}  

namespace content {

class WebRTCInternalsMessageHandler : public WebUIMessageHandler,
                                      public WebRTCInternalsUIObserver{
 public:
  WebRTCInternalsMessageHandler();
  virtual ~WebRTCInternalsMessageHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void OnUpdate(const std::string& command,
                        const base::Value* args) OVERRIDE;

  
  void OnGetAllUpdates(const base::ListValue* list);
  void OnGetAllStats(const base::ListValue* list);
  void OnStartRtpRecording(const base::ListValue* list);
  void OnStopRtpRecording(const base::ListValue* list);

 private:
  DISALLOW_COPY_AND_ASSIGN(WebRTCInternalsMessageHandler);
};

}  

#endif  
