// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef URL_LOADER_HANDLER_H_
#define URL_LOADER_HANDLER_H_

#include <string>
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/url_loader.h"
#include "ppapi/cpp/url_request_info.h"
#include "ppapi/utility/completion_callback_factory.h"
#define READ_BUFFER_SIZE 32768

class URLLoaderHandler {
 public:
  
  
  
  static URLLoaderHandler* Create(pp::Instance* instance_,
                                  const std::string& url);
  
  void Start();

 private:
  URLLoaderHandler(pp::Instance* instance_, const std::string& url);
  ~URLLoaderHandler();

  
  
  
  
  
  void OnOpen(int32_t result);

  
  
  
  void OnRead(int32_t result);

  
  
  void ReadBody();

  
  
  void AppendDataBytes(const char* buffer, int32_t num_bytes);

  
  void ReportResult(const std::string& fname,
                    const std::string& text,
                    bool success);
  
  
  void ReportResultAndDie(const std::string& fname,
                          const std::string& text,
                          bool success);

  pp::Instance* instance_;  
  std::string url_;         
  pp::URLRequestInfo url_request_;
  pp::URLLoader url_loader_;  
  char* buffer_;              
  std::string url_response_body_;  
  pp::CompletionCallbackFactory<URLLoaderHandler> cc_factory_;

  URLLoaderHandler(const URLLoaderHandler&);
  void operator=(const URLLoaderHandler&);
};

#endif  
