// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_HTTP_PIPELINING_COMPATIBILITY_CLIENT_H_
#define CHROME_BROWSER_NET_HTTP_PIPELINING_COMPATIBILITY_CLIENT_H_

#include <string>
#include <vector>

#include "base/memory/scoped_vector.h"
#include "net/base/completion_callback.h"
#include "net/base/io_buffer.h"
#include "net/url_request/url_request.h"
#include "net/url_request/url_request_context.h"

class IOThread;

namespace chrome_browser_net {

struct RequestInfo {
  std::string filename;  
  std::string expected_response;  
};

namespace internal {

class PipelineTestRequest {
 public:
  enum Status {
    STATUS_SUCCESS,
    STATUS_REDIRECTED,         
    STATUS_CERT_ERROR,         
    STATUS_BAD_RESPONSE_CODE,  
    STATUS_NETWORK_ERROR,      
                               
    STATUS_TOO_LARGE,          
                               
    STATUS_TOO_SMALL,          
                               
    STATUS_CONTENT_MISMATCH,   
    STATUS_BAD_HTTP_VERSION,   
    STATUS_CORRUPT_STATS,      
    STATUS_MAX,
  };

  enum Type {
    TYPE_PIPELINED,  
    TYPE_CANARY,     
                     
    TYPE_STATS,      
  };

  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual void OnCanaryFinished(Status status) = 0;

    
    virtual void OnRequestFinished(int request_id, Status status) = 0;

    
    virtual void ReportNetworkError(int request_id, int error_code) = 0;

    
    virtual void ReportResponseCode(int request_id, int response_code) = 0;
  };

  class Factory {
   public:
    virtual ~Factory() {}

    virtual PipelineTestRequest* NewRequest(
        int request_id,
        const std::string& base_url,
        const RequestInfo& info,
        Delegate* delegate,
        net::URLRequestContext* url_request_context,
        Type type) = 0;
  };

  virtual ~PipelineTestRequest() {}

  virtual void Start() = 0;
};

PipelineTestRequest::Status ProcessStatsResponse(
    const std::string& response);

}  

class HttpPipeliningCompatibilityClient
    : public internal::PipelineTestRequest::Delegate {
 public:
  enum Options {
    PIPE_TEST_DEFAULTS,              
    PIPE_TEST_RUN_CANARY_REQUEST,    
                                     
    PIPE_TEST_COLLECT_SERVER_STATS,  
                                     
    PIPE_TEST_CANARY_AND_STATS,      
  };

  HttpPipeliningCompatibilityClient(
      internal::PipelineTestRequest::Factory* factory);
  virtual ~HttpPipeliningCompatibilityClient();

  
  
  
  
  
  
  void Start(const std::string& base_url,
             std::vector<RequestInfo>& requests,
             Options options,
             const net::CompletionCallback& callback,
             net::URLRequestContext* url_request_context);

 private:
  
  void StartTestRequests();

  
  std::string GetMetricName(int request_id, const char* description);

  
  virtual void OnCanaryFinished(
      internal::PipelineTestRequest::Status status) OVERRIDE;
  virtual void OnRequestFinished(
      int request_id,
      internal::PipelineTestRequest::Status status) OVERRIDE;
  virtual void ReportNetworkError(int request_id, int error_code) OVERRIDE;
  virtual void ReportResponseCode(int request_id, int response_code) OVERRIDE;

  scoped_ptr<net::HttpTransactionFactory> http_transaction_factory_;
  scoped_ptr<net::URLRequestContext> url_request_context_;
  scoped_ptr<internal::PipelineTestRequest::Factory> factory_;
  ScopedVector<internal::PipelineTestRequest> requests_;
  scoped_ptr<internal::PipelineTestRequest> canary_request_;
  net::CompletionCallback finished_callback_;
  size_t num_finished_;
  size_t num_succeeded_;
};

void CollectPipeliningCapabilityStatsOnUIThread(
    const std::string& pipeline_test_server, IOThread* io_thread);

}  

#endif  
