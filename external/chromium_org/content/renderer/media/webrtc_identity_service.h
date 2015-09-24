// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_WEBRTC_IDENTITY_SERVICE_H_
#define CONTENT_RENDERER_MEDIA_WEBRTC_IDENTITY_SERVICE_H_

#include <deque>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "content/common/content_export.h"
#include "content/public/renderer/render_process_observer.h"
#include "url/gurl.h"

namespace content {

class CONTENT_EXPORT WebRTCIdentityService : public RenderProcessObserver {
 public:
  typedef base::Callback<
      void(const std::string& certificate, const std::string& private_key)>
      SuccessCallback;

  typedef base::Callback<void(int error)> FailureCallback;

  WebRTCIdentityService();
  virtual ~WebRTCIdentityService();

  
  
  
  
  
  
  
  
  
  
  
  int RequestIdentity(const GURL& origin,
                      const std::string& identity_name,
                      const std::string& common_name,
                      const SuccessCallback& success_callback,
                      const FailureCallback& failure_callback);

  
  
  
  
  
  void CancelRequest(int request_id);

 protected:
  
  virtual bool Send(IPC::Message* message);
  
  virtual bool OnControlMessageReceived(const IPC::Message& message) OVERRIDE;

 private:
  struct RequestInfo {
    RequestInfo(int request_id,
                const GURL& origin,
                const std::string& identity_name,
                const std::string& common_name,
                const SuccessCallback& success_callback,
                const FailureCallback& failure_callback);
    ~RequestInfo();

    int request_id;
    GURL origin;
    std::string identity_name;
    std::string common_name;
    SuccessCallback success_callback;
    FailureCallback failure_callback;
  };

  
  void OnIdentityReady(int request_id,
                       const std::string& certificate,
                       const std::string& private_key);
  void OnRequestFailed(int request_id, int error);

  void SendRequest(const RequestInfo& request_info);
  void OnOutstandingRequestReturned();

  std::deque<RequestInfo> pending_requests_;
  int next_request_id_;

  DISALLOW_COPY_AND_ASSIGN(WebRTCIdentityService);
};

}  

#endif  
