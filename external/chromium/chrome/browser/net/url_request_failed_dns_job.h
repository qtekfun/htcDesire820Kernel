// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_URL_REQUEST_FAILED_DNS_JOB_H_
#define CHROME_BROWSER_NET_URL_REQUEST_FAILED_DNS_JOB_H_
#pragma once

#include "base/task.h"
#include "net/url_request/url_request_job.h"

class URLRequestFailedDnsJob : public net::URLRequestJob {
 public:
  explicit URLRequestFailedDnsJob(net::URLRequest* request);

  virtual void Start();

  static net::URLRequestJob* Factory(net::URLRequest* request,
                                     const std::string& scheme);

  
  static const char kTestUrl[];

  
  static void AddUrlHandler();

 private:
  ~URLRequestFailedDnsJob();

  
  void StartAsync();

  ScopedRunnableMethodFactory<URLRequestFailedDnsJob> method_factory_;
};

#endif  
