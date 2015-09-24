// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_GVIEW_REQUEST_INTERCEPTOR_H__
#define CHROME_BROWSER_CHROMEOS_GVIEW_REQUEST_INTERCEPTOR_H__
#pragma once

#include <string>
#include "base/hash_tables.h"
#include "net/url_request/url_request.h"

template <typename T> struct DefaultSingletonTraits;

namespace chromeos {

class GViewRequestInterceptor : public net::URLRequest::Interceptor {
 public:
  
  
  virtual net::URLRequestJob* MaybeIntercept(net::URLRequest* request);

  
  
  
  virtual net::URLRequestJob* MaybeInterceptResponse(net::URLRequest* request);

  
  static GViewRequestInterceptor* GetInstance();

 private:
  friend struct DefaultSingletonTraits<GViewRequestInterceptor>;

  GViewRequestInterceptor();
  virtual ~GViewRequestInterceptor();

  
  base::hash_set<std::string> supported_mime_types_;
};

}  

#endif  
