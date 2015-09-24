// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_NET_PRECONNECT_H_
#define CHROME_BROWSER_NET_PRECONNECT_H_

#include "chrome/browser/net/url_info.h"

class GURL;

namespace net {
class URLRequestContextGetter;
}

namespace chrome_browser_net {

void PreconnectOnUIThread(const GURL& url,
                          const GURL& first_party_for_cookies,
                          UrlInfo::ResolutionMotivation motivation,
                          int count,
                          net::URLRequestContextGetter* getter);

void PreconnectOnIOThread(const GURL& url,
                          const GURL& first_party_for_cookies,
                          UrlInfo::ResolutionMotivation motivation,
                          int count,
                          net::URLRequestContextGetter* getter);

}  

#endif  
