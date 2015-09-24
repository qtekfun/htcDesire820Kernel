// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_PUBLIC_COMMON_RESOURCE_RESPONSE_H_
#define CONTENT_PUBLIC_COMMON_RESOURCE_RESPONSE_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"
#include "net/url_request/url_request_status.h"
#include "url/gurl.h"
#include "webkit/common/resource_response_info.h"

namespace content {

struct ResourceResponseHead : webkit_glue::ResourceResponseInfo {
  
  int error_code;
  
  base::TimeTicks request_start;
  
  base::TimeTicks response_start;
};

struct SyncLoadResult : ResourceResponseHead {
  
  GURL final_url;

  
  std::string data;
};

struct CONTENT_EXPORT ResourceResponse
    : public base::RefCounted<ResourceResponse> {
 public:
  ResourceResponseHead head;

 private:
  friend class base::RefCounted<ResourceResponse>;
  ~ResourceResponse() {}
};

}  

#endif  
