// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_HTTP_POST_PROVIDER_INTERFACE_H_
#define SYNC_INTERNAL_API_PUBLIC_HTTP_POST_PROVIDER_INTERFACE_H_

#include <string>

#include "sync/base/sync_export.h"

namespace syncer {

class SYNC_EXPORT_PRIVATE HttpPostProviderInterface {
 public:
  
  virtual void SetExtraRequestHeaders(const char* headers) = 0;

  
  virtual void SetURL(const char* url, int port) = 0;

  
  
  
  
  
  virtual void SetPostPayload(const char* content_type,
                              int content_length,
                              const char* content) = 0;

  
  
  virtual bool MakeSynchronousPost(int* error_code, int* response_code) = 0;

  
  
  
  virtual int GetResponseContentLength() const = 0;

  
  
  
  virtual const char* GetResponseContent() const = 0;

  
  
  virtual const std::string GetResponseHeaderValue(
      const std::string& name) const = 0;

  
  
  virtual void Abort() = 0;

 protected:
  virtual ~HttpPostProviderInterface() {}
};

}  

#endif  
