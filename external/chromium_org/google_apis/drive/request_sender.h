// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_DRIVE_REQUEST_SENDER_H_
#define GOOGLE_APIS_DRIVE_REQUEST_SENDER_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread_checker.h"
#include "google_apis/drive/gdata_errorcode.h"

namespace base {
class SequencedTaskRunner;
}

namespace net {
class URLRequestContextGetter;
}

namespace google_apis {

class AuthenticatedRequestInterface;
class AuthServiceInterface;

class RequestSender {
 public:
  
  
  
  
  
  
  
  
  
  
  
  RequestSender(AuthServiceInterface* auth_service,
                net::URLRequestContextGetter* url_request_context_getter,
                base::SequencedTaskRunner* blocking_task_runner,
                const std::string& custom_user_agent);
  ~RequestSender();

  AuthServiceInterface* auth_service() { return auth_service_.get(); }

  net::URLRequestContextGetter* url_request_context_getter() const {
    return url_request_context_getter_;
  }

  base::SequencedTaskRunner* blocking_task_runner() const {
    return blocking_task_runner_.get();
  }

  
  
  
  
  
  
  
  
  base::Closure StartRequestWithRetry(AuthenticatedRequestInterface* request);

  
  
  void RequestFinished(AuthenticatedRequestInterface* request);

 private:
  
  void OnAccessTokenFetched(
      const base::WeakPtr<AuthenticatedRequestInterface>& request,
      GDataErrorCode error,
      const std::string& access_token);

  
  
  void RetryRequest(AuthenticatedRequestInterface* request);

  
  
  void CancelRequest(
      const base::WeakPtr<AuthenticatedRequestInterface>& request);

  scoped_ptr<AuthServiceInterface> auth_service_;
  scoped_refptr<net::URLRequestContextGetter> url_request_context_getter_;
  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;

  std::set<AuthenticatedRequestInterface*> in_flight_requests_;
  const std::string custom_user_agent_;

  base::ThreadChecker thread_checker_;

  
  
  base::WeakPtrFactory<RequestSender> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(RequestSender);
};

}  

#endif  
