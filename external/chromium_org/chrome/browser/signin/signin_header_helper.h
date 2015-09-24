// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_SIGNIN_HEADER_HELPER_H_
#define CHROME_BROWSER_SIGNIN_SIGNIN_HEADER_HELPER_H_

namespace net {
class URLRequest;
}
class GURL;
class ProfileIOData;

namespace signin {

void AppendMirrorRequestHeaderIfPossible(
    net::URLRequest* request,
    const GURL& redirect_url,
    ProfileIOData* io_data,
    int child_id,
    int route_id);

void ProcessMirrorResponseHeaderIfExists(
    net::URLRequest* request,
    ProfileIOData* io_data,
    int child_id,
    int route_id);

};  

#endif  
