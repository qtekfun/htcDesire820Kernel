// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_REQUEST_INFO_H__
#define NET_HTTP_HTTP_REQUEST_INFO_H__
#pragma once

#include <string>
#include "base/memory/ref_counted.h"
#include "googleurl/src/gurl.h"
#include "net/base/request_priority.h"
#include "net/base/upload_data.h"
#include "net/http/http_request_headers.h"

namespace net {

struct HttpRequestInfo {
  enum RequestMotivation{
    
    PRECONNECT_MOTIVATED,  
    OMNIBOX_MOTIVATED,     
    NORMAL_MOTIVATION,     
    EARLY_LOAD_MOTIVATED,  
                           
                           
  };

  HttpRequestInfo();
  ~HttpRequestInfo();

  
  GURL url;

  
  GURL referrer;

  
  std::string method;

  
  HttpRequestHeaders extra_headers;

  
  scoped_refptr<UploadData> upload_data;

  
  int load_flags;

  
  RequestPriority priority;

  
  RequestMotivation motivation;

  
  
  uint64 request_id;

#ifdef ANDROID
  bool valid_uid;
  uid_t calling_uid;
#endif

};

}  

#endif  
