// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_NET_URL_REQUEST_SLOW_DOWNLOAD_JOB_H_
#define CONTENT_TEST_NET_URL_REQUEST_SLOW_DOWNLOAD_JOB_H_

#include <set>
#include <string>

#include "base/lazy_instance.h"
#include "base/memory/weak_ptr.h"
#include "net/url_request/url_request_job.h"

namespace content {

class URLRequestSlowDownloadJob : public net::URLRequestJob {
 public:
  
  static const char kUnknownSizeUrl[];
  static const char kKnownSizeUrl[];
  static const char kFinishDownloadUrl[];
  static const char kErrorDownloadUrl[];

  
  static const int kFirstDownloadSize;
  static const int kSecondDownloadSize;

  
  
  void CheckDoneStatus();

  
  virtual void Start() OVERRIDE;
  virtual bool GetMimeType(std::string* mime_type) const OVERRIDE;
  virtual void GetResponseInfo(net::HttpResponseInfo* info) OVERRIDE;
  virtual bool ReadRawData(net::IOBuffer* buf,
                           int buf_size,
                           int *bytes_read) OVERRIDE;

  static net::URLRequestJob* Factory(net::URLRequest* request,
                                     net::NetworkDelegate* network_delegate,
                                     const std::string& scheme);

  
  
  static size_t NumberOutstandingRequests();

  
  static void AddUrlHandler();

 private:
  URLRequestSlowDownloadJob(net::URLRequest* request,
                            net::NetworkDelegate* network_delegate);
  virtual ~URLRequestSlowDownloadJob();

  
  
  enum ReadStatus {
    
    BUFFER_FILLED,

    
    
    REQUEST_BLOCKED,

    
    
    REQUEST_COMPLETE
  };
  ReadStatus FillBufferHelper(
      net::IOBuffer* buf,
      int buf_size,
      int* bytes_written);

  void GetResponseInfoConst(net::HttpResponseInfo* info) const;

  
  
  static void FinishPendingRequests();
  static void ErrorPendingRequests();
  typedef std::set<URLRequestSlowDownloadJob*> SlowJobsSet;
  static base::LazyInstance<SlowJobsSet>::Leaky pending_requests_;

  void StartAsync();

  void set_should_finish_download() { should_finish_download_ = true; }
  void set_should_error_download() { should_error_download_ = true; }

  int bytes_already_sent_;
  bool should_error_download_;
  bool should_finish_download_;
  scoped_refptr<net::IOBuffer> buffer_;
  int buffer_size_;

  base::WeakPtrFactory<URLRequestSlowDownloadJob> weak_factory_;
};

}  

#endif  
