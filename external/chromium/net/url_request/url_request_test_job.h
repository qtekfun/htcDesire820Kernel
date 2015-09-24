// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_TEST_JOB_H_
#define NET_URL_REQUEST_URL_REQUEST_TEST_JOB_H_
#pragma once

#include <string>

#include "net/url_request/url_request.h"
#include "net/url_request/url_request_job.h"

namespace net {

class URLRequestTestJob : public URLRequestJob {
 public:
  
  
  explicit URLRequestTestJob(URLRequest* request);

  
  
  explicit URLRequestTestJob(URLRequest* request, bool auto_advance);

  
  
  
  explicit URLRequestTestJob(URLRequest* request,
                             const std::string& response_headers,
                             const std::string& response_data,
                             bool auto_advance);

  
  
  
  static GURL test_url_1();
  static GURL test_url_2();
  static GURL test_url_3();
  static GURL test_url_error();

  
  static std::string test_data_1();
  static std::string test_data_2();
  static std::string test_data_3();

  
  static std::string test_headers();

  
  static std::string test_redirect_headers();

  
  static std::string test_error_headers();

  
  
  
  static bool ProcessOnePendingMessage();

  
  
  
  
  bool auto_advance() { return auto_advance_; }
  void set_auto_advance(bool auto_advance) { auto_advance_ = auto_advance; }

  
  static URLRequest::ProtocolFactory Factory;

  
  virtual void Start();
  virtual bool ReadRawData(IOBuffer* buf, int buf_size, int *bytes_read);
  virtual void Kill();
  virtual bool GetMimeType(std::string* mime_type) const;
  virtual void GetResponseInfo(HttpResponseInfo* info);
  virtual int GetResponseCode() const;
  virtual bool IsRedirectResponse(GURL* location, int* http_status_code);

 protected:
  
  
  enum Stage { WAITING, DATA_AVAILABLE, ALL_DATA, DONE };

  virtual ~URLRequestTestJob();

  
  
  void ProcessNextOperation();

  
  void AdvanceJob();

  
  virtual void StartAsync();

  bool auto_advance_;

  Stage stage_;

  
  
  scoped_refptr<HttpResponseHeaders> response_headers_;

  
  
  std::string response_data_;

  
  int offset_;

  
  IOBuffer* async_buf_;
  int async_buf_size_;
};

}  

#endif  
