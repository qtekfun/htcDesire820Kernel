// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_URL_LOADER_H_
#define PPAPI_CPP_URL_LOADER_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/cpp/resource.h"

namespace pp {

class CompletionCallback;
class InstanceHandle;
class URLRequestInfo;
class URLResponseInfo;

class URLLoader : public Resource {
 public:
  
  
  URLLoader() {}

  
  
  
  
  
  explicit URLLoader(PP_Resource resource);

  
  
  
  
  
  explicit URLLoader(const InstanceHandle& instance);

  
  
  
  URLLoader(const URLLoader& other);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t Open(const URLRequestInfo& request_info,
               const CompletionCallback& cc);

  
  
  
  
  
  
  
  
  
  
  
  int32_t FollowRedirect(const CompletionCallback& cc);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool GetUploadProgress(int64_t* bytes_sent,
                         int64_t* total_bytes_to_be_sent) const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool GetDownloadProgress(int64_t* bytes_received,
                           int64_t* total_bytes_to_be_received) const;

  
  
  
  
  
  
  
  URLResponseInfo GetResponseInfo() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t ReadResponseBody(void* buffer,
                           int32_t bytes_to_read,
                           const CompletionCallback& cc);

  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t FinishStreamingToFile(const CompletionCallback& cc);

  
  
  
  
  
  
  
  
  void Close();
};

}  

#endif  
