// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_URL_REQUEST_SLOW_HTTP_JOB_H_
#define CHROME_BROWSER_NET_URL_REQUEST_SLOW_HTTP_JOB_H_
#pragma once

#include "base/timer.h"
#include "chrome/browser/net/url_request_mock_http_job.h"

class URLRequestSlowHTTPJob : public URLRequestMockHTTPJob {
 public:
  URLRequestSlowHTTPJob(net::URLRequest* request, const FilePath& file_path);

  static const int kDelayMs;

  static net::URLRequest::ProtocolFactory Factory;

  
  static void AddUrlHandler(const FilePath& base_path);

  
  static GURL GetMockUrl(const FilePath& path);

  virtual void Start();

 private:
  ~URLRequestSlowHTTPJob();

  void RealStart();

  base::OneShotTimer<URLRequestSlowHTTPJob> delay_timer_;

  
  
  static FilePath base_path_;
};

#endif  
