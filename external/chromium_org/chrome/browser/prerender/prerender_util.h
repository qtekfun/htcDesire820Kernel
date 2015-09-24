// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRERENDER_PRERENDER_UTIL_H_
#define CHROME_BROWSER_PRERENDER_PRERENDER_UTIL_H_

#include "base/basictypes.h"
#include "url/gurl.h"
#include "webkit/common/resource_type.h"

namespace net {
class HttpResponseHeaders;
class URLRequest;
}

namespace prerender {

extern const char kChromeNavigateExtraDataKey[];

const uint8 kNoExperiment = 0;

bool MaybeGetQueryStringBasedAliasURL(const GURL& url, GURL* alias_url);

uint8 GetQueryStringBasedExperiment(const GURL& url);

bool IsGoogleDomain(const GURL& url);

bool IsGoogleSearchResultURL(const GURL& url);

bool IsNoSwapInExperiment(uint8 experiment_id);

bool IsControlGroupExperiment(uint8 experiment_id);

void GatherPagespeedData(const ResourceType::Type resource_type,
                         const GURL& request_url,
                         const net::HttpResponseHeaders* response_headers);

void URLRequestResponseStarted(net::URLRequest* request);

void ReportPrerenderExternalURL();

void ReportUnsupportedPrerenderScheme(const GURL& url);

}  

#endif  
