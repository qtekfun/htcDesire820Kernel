// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_NET_URL_REQUEST_ABORT_ON_END_JOB_H_
#define CONTENT_TEST_NET_URL_REQUEST_ABORT_ON_END_JOB_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "net/url_request/url_request_job.h"

namespace content {


class URLRequestAbortOnEndJob : public net::URLRequestJob {
 public:
  static const char k400AbortOnEndUrl[];

  URLRequestAbortOnEndJob(net::URLRequest* request,
                          net::NetworkDelegate* network_delegate);

  
  virtual void Start() OVERRIDE;
  virtual bool GetMimeType(std::string* mime_type) const OVERRIDE;
  virtual void GetResponseInfo(net::HttpResponseInfo* info) OVERRIDE;
  virtual bool ReadRawData(net::IOBuffer* buf,
                           int buf_size,
                           int* bytes_read) OVERRIDE;

  static void AddUrlHandler();

 private:
  virtual ~URLRequestAbortOnEndJob();

  void GetResponseInfoConst(net::HttpResponseInfo* info) const;
  void StartAsync();

  bool sent_data_;

  base::WeakPtrFactory<URLRequestAbortOnEndJob> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestAbortOnEndJob);
};

}  

#endif  
