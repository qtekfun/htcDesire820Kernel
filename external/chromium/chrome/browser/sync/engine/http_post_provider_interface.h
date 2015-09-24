// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_HTTP_POST_PROVIDER_INTERFACE_H_
#define CHROME_BROWSER_SYNC_ENGINE_HTTP_POST_PROVIDER_INTERFACE_H_
#pragma once

#include <string>

namespace sync_api {

class HttpPostProviderInterface {
 public:
  virtual ~HttpPostProviderInterface() {}

  
  
  virtual void SetUserAgent(const char* user_agent) = 0;

  
  virtual void SetExtraRequestHeaders(const char* headers) = 0;

  
  virtual void SetURL(const char* url, int port) = 0;

  
  
  
  
  
  virtual void SetPostPayload(const char* content_type,
                              int content_length,
                              const char* content) = 0;

  
  
  virtual bool MakeSynchronousPost(int* os_error_code, int* response_code) = 0;

  
  
  
  virtual int GetResponseContentLength() const = 0;

  
  
  
  virtual const char* GetResponseContent() const = 0;

  
  
  virtual const std::string GetResponseHeaderValue(
      const std::string& name) const = 0;

  
  
  virtual void Abort() = 0;
};

}  

#endif  
