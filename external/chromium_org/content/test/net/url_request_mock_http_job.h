// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_NET_URL_REQUEST_MOCK_HTTP_JOB_H_
#define CONTENT_TEST_NET_URL_REQUEST_MOCK_HTTP_JOB_H_

#include <string>

#include "net/url_request/url_request_file_job.h"
#include "net/url_request/url_request_job_factory.h"

namespace base {
class FilePath;
}

namespace content {

class URLRequestMockHTTPJob : public net::URLRequestFileJob {
 public:
  URLRequestMockHTTPJob(net::URLRequest* request,
                        net::NetworkDelegate* network_delegate,
                        const base::FilePath& file_path);

  virtual bool GetMimeType(std::string* mime_type) const OVERRIDE;
  virtual int GetResponseCode() const OVERRIDE;
  virtual bool GetCharset(std::string* charset) OVERRIDE;
  virtual void GetResponseInfo(net::HttpResponseInfo* info) OVERRIDE;
  virtual bool IsRedirectResponse(GURL* location,
                                  int* http_status_code) OVERRIDE;

  
  static void AddUrlHandler(const base::FilePath& base_path);

  
  
  static void AddHostnameToFileHandler(const std::string& hostname,
                                       const base::FilePath& file);

  
  
  static GURL GetMockUrl(const base::FilePath& path);

  
  
  static GURL GetMockViewSourceUrl(const base::FilePath& path);

  
  
  
  
  static scoped_ptr<net::URLRequestJobFactory::ProtocolHandler>
  CreateProtocolHandler(const base::FilePath& base_path);

  
  
  
  static scoped_ptr<net::URLRequestJobFactory::ProtocolHandler>
  CreateProtocolHandlerForSingleFile(const base::FilePath& file);

 protected:
  virtual ~URLRequestMockHTTPJob();

 private:
  void GetResponseInfoConst(net::HttpResponseInfo* info) const;
};

}  

#endif  
