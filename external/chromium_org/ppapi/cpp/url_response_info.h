// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_URL_RESPONSE_INFO_H_
#define PPAPI_CPP_URL_RESPONSE_INFO_H_

#include "ppapi/c/ppb_url_response_info.h"
#include "ppapi/cpp/resource.h"
#include "ppapi/cpp/var.h"

namespace pp {

class FileRef;

class URLResponseInfo : public Resource {
 public:
  
  
  URLResponseInfo() {}

  
  
  
  
  
  URLResponseInfo(PassRef, PP_Resource resource);

  
  URLResponseInfo(const URLResponseInfo& other);

  
  
  
  
  
  
  
  
  Var GetProperty(PP_URLResponseProperty property) const;

  
  
  
  
  
  
  
  
  
  
  
  FileRef GetBodyAsFileRef() const;

  
  
  
  
  
  
  Var GetURL() const {
    return GetProperty(PP_URLRESPONSEPROPERTY_URL);
  }

  
  
  
  
  
  
  Var GetRedirectURL() const {
    return GetProperty(PP_URLRESPONSEPROPERTY_REDIRECTURL);
  }

  
  
  
  
  
  
  Var GetRedirectMethod() const {
    return GetProperty(PP_URLRESPONSEPROPERTY_REDIRECTMETHOD);
  }

  
  
  
  
  
  int32_t GetStatusCode() const {
    return GetProperty(PP_URLRESPONSEPROPERTY_STATUSCODE).AsInt();
  }

  
  
  
  
  
  
  Var GetStatusLine() const {
    return GetProperty(PP_URLRESPONSEPROPERTY_STATUSLINE);
  }

  
  
  
  
  
  
  Var GetHeaders() const {
    return GetProperty(PP_URLRESPONSEPROPERTY_HEADERS);
  }
};

}  

#endif  
