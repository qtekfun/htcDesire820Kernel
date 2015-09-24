// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_URL_REQUEST_MOCK_HTTP_JOB_H_
#define CHROME_BROWSER_NET_URL_REQUEST_MOCK_HTTP_JOB_H_
#pragma once

#include <string>

#include "net/url_request/url_request_file_job.h"

class FilePath;

class URLRequestMockHTTPJob : public net::URLRequestFileJob {
 public:
  URLRequestMockHTTPJob(net::URLRequest* request, const FilePath& file_path);

  virtual bool GetMimeType(std::string* mime_type) const;
  virtual bool GetCharset(std::string* charset);
  virtual void GetResponseInfo(net::HttpResponseInfo* info);
  virtual bool IsRedirectResponse(GURL* location, int* http_status_code);

  static net::URLRequest::ProtocolFactory Factory;

  
  static void AddUrlHandler(const FilePath& base_path);

  
  static GURL GetMockUrl(const FilePath& path);

  
  
  static GURL GetMockViewSourceUrl(const FilePath& path);

 protected:
  virtual ~URLRequestMockHTTPJob() { }

  static FilePath GetOnDiskPath(const FilePath& base_path,
                                net::URLRequest* request,
                                const std::string& scheme);

 private:
  void GetResponseInfoConst(net::HttpResponseInfo* info) const;

  
  
  static FilePath base_path_;
};

#endif  
