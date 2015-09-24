// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_RENDERER_HOST_CHROME_URL_REQUEST_USER_DATA_H_
#define CHROME_BROWSER_RENDERER_HOST_CHROME_URL_REQUEST_USER_DATA_H_

#include "base/supports_user_data.h"

namespace net {
class URLRequest;
}

class ChromeURLRequestUserData : public base::SupportsUserData::Data {
 public:
  bool is_prerender() const { return is_prerender_; }
  void set_is_prerender(bool is_prerender) { is_prerender_ = is_prerender; }

  
  
  
  
  static ChromeURLRequestUserData* Create(net::URLRequest* request);

  
  
  static void Delete(net::URLRequest* request);

  
  
  static ChromeURLRequestUserData* Get(const net::URLRequest* request);

 private:
  ChromeURLRequestUserData();

  bool is_prerender_;

  DISALLOW_COPY_AND_ASSIGN(ChromeURLRequestUserData);
};

#endif  
