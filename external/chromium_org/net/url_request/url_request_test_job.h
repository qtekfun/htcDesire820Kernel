// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_TEST_JOB_H_
#define NET_URL_REQUEST_URL_REQUEST_TEST_JOB_H_

#include <string>

#include "base/memory/weak_ptr.h"
#include "net/base/load_timing_info.h"
#include "net/url_request/url_request.h"
#include "net/url_request/url_request_job.h"

namespace net {

class NET_EXPORT_PRIVATE URLRequestTestJob : public URLRequestJob {
 public:
  
  
  URLRequestTestJob(URLRequest* request, NetworkDelegate* network_delegate);

  
  
  URLRequestTestJob(URLRequest* request,
                    NetworkDelegate* network_delegate,
                    bool auto_advance);

  
  
  
  URLRequestTestJob(URLRequest* request,
                    net::NetworkDelegate* network_delegate,
                    const std::string& response_headers,
                    const std::string& response_data,
                    bool auto_advance);

  
  
  
  static GURL test_url_1();
  static GURL test_url_2();
  static GURL test_url_3();
  static GURL test_url_4();
  static GURL test_url_error();
  static GURL test_url_redirect_to_url_2();

  
  static std::string test_data_1();
  static std::string test_data_2();
  static std::string test_data_3();
  static std::string test_data_4();

  
  static std::string test_headers();

  
  static std::string test_redirect_headers();

  
  static std::string test_redirect_to_url_2_headers();

  
  static std::string test_error_headers();

  
  
  
  static bool ProcessOnePendingMessage();

  
  
  
  
  bool auto_advance() { return auto_advance_; }
  void set_auto_advance(bool auto_advance) { auto_advance_ = auto_advance; }

  void set_load_timing_info(const LoadTimingInfo& load_timing_info) {
    load_timing_info_ = load_timing_info;
  }

  RequestPriority priority() const { return priority_; }

  
  static URLRequest::ProtocolFactory Factory;

  
  virtual void SetPriority(RequestPriority priority) OVERRIDE;
  virtual void Start() OVERRIDE;
  virtual bool ReadRawData(IOBuffer* buf,
                           int buf_size,
                           int *bytes_read) OVERRIDE;
  virtual void Kill() OVERRIDE;
  virtual bool GetMimeType(std::string* mime_type) const OVERRIDE;
  virtual void GetResponseInfo(HttpResponseInfo* info) OVERRIDE;
  virtual void GetLoadTimingInfo(
      LoadTimingInfo* load_timing_info) const OVERRIDE;
  virtual int GetResponseCode() const OVERRIDE;
  virtual bool IsRedirectResponse(GURL* location,
                                  int* http_status_code) OVERRIDE;

 protected:
  
  
  
  
  virtual bool NextReadAsync();

  
  
  enum Stage { WAITING, DATA_AVAILABLE, ALL_DATA, DONE };

  virtual ~URLRequestTestJob();

  
  
  void ProcessNextOperation();

  
  void AdvanceJob();

  
  virtual void StartAsync();

  bool auto_advance_;

  Stage stage_;

  RequestPriority priority_;

  
  
  scoped_refptr<HttpResponseHeaders> response_headers_;

  
  
  std::string response_data_;

  
  int offset_;

  
  IOBuffer* async_buf_;
  int async_buf_size_;

  LoadTimingInfo load_timing_info_;

  base::WeakPtrFactory<URLRequestTestJob> weak_factory_;
};

}  

#endif  
