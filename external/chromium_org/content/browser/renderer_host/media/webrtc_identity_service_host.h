// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_MEDIA_WEBRTC_IDENTITY_SERVICE_HOST_H_
#define CONTENT_BROWSER_RENDERER_HOST_MEDIA_WEBRTC_IDENTITY_SERVICE_HOST_H_

#include <string>

#include "base/basictypes.h"
#include "content/common/content_export.h"
#include "content/public/browser/browser_message_filter.h"

class GURL;

namespace content {

class WebRTCIdentityStore;

class CONTENT_EXPORT WebRTCIdentityServiceHost : public BrowserMessageFilter {
 public:
  WebRTCIdentityServiceHost(int renderer_process_id,
                            scoped_refptr<WebRTCIdentityStore> identity_store);

 protected:
  virtual ~WebRTCIdentityServiceHost();

  
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;

 private:
  
  
  void OnComplete(int sequence_number,
                  int status,
                  const std::string& certificate,
                  const std::string& private_key);

  
  
  
  
  
  void OnRequestIdentity(int sequence_number,
                         const GURL& origin,
                         const std::string& identity_name,
                         const std::string& common_name);

  void OnCancelRequest();

  void SendErrorMessage(int sequence_number, int error);

  int renderer_process_id_;
  base::Closure cancel_callback_;
  scoped_refptr<WebRTCIdentityStore> identity_store_;
  base::WeakPtrFactory<WebRTCIdentityServiceHost> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(WebRTCIdentityServiceHost);
};

}  

#endif  
