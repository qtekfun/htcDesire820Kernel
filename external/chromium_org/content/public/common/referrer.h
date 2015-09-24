// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_REFERRER_H_
#define CONTENT_PUBLIC_COMMON_REFERRER_H_

#include "content/common/content_export.h"
#include "third_party/WebKit/public/platform/WebReferrerPolicy.h"
#include "url/gurl.h"

namespace content {

struct CONTENT_EXPORT Referrer {
  Referrer(const GURL& url, blink::WebReferrerPolicy policy) : url(url),
                                                                policy(policy) {
  }
  Referrer() : policy(blink::WebReferrerPolicyDefault) {
  }

  GURL url;
  blink::WebReferrerPolicy policy;
};

}  

#endif  
