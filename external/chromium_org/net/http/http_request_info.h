// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_REQUEST_INFO_H__
#define NET_HTTP_HTTP_REQUEST_INFO_H__

#include <string>

#include "base/basictypes.h"
#include "net/base/net_export.h"
#include "net/base/privacy_mode.h"
#include "net/http/http_request_headers.h"
#include "url/gurl.h"

namespace net {

class UploadDataStream;

struct NET_EXPORT HttpRequestInfo {
  enum RequestMotivation{
    
    PRECONNECT_MOTIVATED,  
    OMNIBOX_MOTIVATED,     
    NORMAL_MOTIVATION,     
    EARLY_LOAD_MOTIVATED,  
                           
                           
  };

  HttpRequestInfo();
  ~HttpRequestInfo();

  
  GURL url;

  
  std::string method;

  
  HttpRequestHeaders extra_headers;

  
  UploadDataStream* upload_data_stream;

  
  int load_flags;

  
  RequestMotivation motivation;

  
  
  PrivacyMode privacy_mode;
};

}  

#endif  
