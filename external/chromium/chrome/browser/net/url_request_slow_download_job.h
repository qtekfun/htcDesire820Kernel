// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_URL_REQUEST_SLOW_DOWNLOAD_JOB_H_
#define CHROME_BROWSER_NET_URL_REQUEST_SLOW_DOWNLOAD_JOB_H_
#pragma once

#include <string>
#include <vector>

#include "base/task.h"
#include "net/url_request/url_request_job.h"

class URLRequestSlowDownloadJob : public net::URLRequestJob {
 public:
  explicit URLRequestSlowDownloadJob(net::URLRequest* request);

  
  
  void CheckDoneStatus();

  
  virtual void Start();
  virtual bool GetMimeType(std::string* mime_type) const;
  virtual void GetResponseInfo(net::HttpResponseInfo* info);
  virtual bool ReadRawData(net::IOBuffer* buf, int buf_size, int *bytes_read);

  static net::URLRequestJob* Factory(net::URLRequest* request,
                                     const std::string& scheme);

  
  static const char kUnknownSizeUrl[];
  static const char kKnownSizeUrl[];
  static const char kFinishDownloadUrl[];

  
  static void AddUrlHandler();

 private:
  virtual ~URLRequestSlowDownloadJob();

  void GetResponseInfoConst(net::HttpResponseInfo* info) const;

  
  
  static void FinishPendingRequests();
  static std::vector<URLRequestSlowDownloadJob*> kPendingRequests;

  void StartAsync();

  void set_should_finish_download() { should_finish_download_ = true; }

  int first_download_size_remaining_;
  bool should_finish_download_;
  bool should_send_second_chunk_;

  ScopedRunnableMethodFactory<URLRequestSlowDownloadJob> method_factory_;
};

#endif  
