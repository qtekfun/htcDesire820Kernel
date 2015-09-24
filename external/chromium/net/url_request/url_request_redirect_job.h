// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_REDIRECT_JOB_H_
#define NET_URL_REQUEST_URL_REQUEST_REDIRECT_JOB_H_
#pragma once

#include "base/task.h"
#include "net/url_request/url_request_job.h"

class GURL;

namespace net {

class URLRequestRedirectJob : public URLRequestJob {
 public:
  
  URLRequestRedirectJob(URLRequest* request, const GURL& redirect_destination);

  virtual void Start();
  virtual bool IsRedirectResponse(GURL* location, int* http_status_code);

 private:
  virtual ~URLRequestRedirectJob();

  void StartAsync();

  GURL redirect_destination_;

  ScopedRunnableMethodFactory<URLRequestRedirectJob> method_factory_;
};

}  

#endif  
