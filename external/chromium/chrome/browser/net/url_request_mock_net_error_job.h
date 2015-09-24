// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_NET_URL_REQUEST_MOCK_NET_ERROR_JOB_H_
#define CHROME_BROWSER_NET_URL_REQUEST_MOCK_NET_ERROR_JOB_H_
#pragma once

#include "chrome/browser/net/url_request_mock_http_job.h"

class URLRequestMockNetErrorJob : public URLRequestMockHTTPJob {
 public:
  URLRequestMockNetErrorJob(net::URLRequest* request,
                            const std::vector<int>& errors,
                            net::X509Certificate* ssl_cert,
                            const FilePath& file_path);

  virtual void Start();
  virtual void ContinueDespiteLastError();

  
  
  
  
  
  
  static void AddMockedURL(const GURL& url,
                           const std::wstring& base,
                           const std::vector<int>& errors,
                           net::X509Certificate* ssl_cert);

  
  static void RemoveMockedURL(const GURL& url);

 private:
  ~URLRequestMockNetErrorJob();

  static net::URLRequest::ProtocolFactory Factory;

  void StartAsync();

  
  std::vector<int> errors_;

  
  scoped_refptr<net::X509Certificate> ssl_cert_;

  struct MockInfo;
  typedef std::map<GURL, MockInfo> URLMockInfoMap;
  static URLMockInfoMap url_mock_info_map_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestMockNetErrorJob);
};

#endif  
