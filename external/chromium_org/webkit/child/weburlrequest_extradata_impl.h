// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_CHILD_WEBURLREQUEST_EXTRADATA_IMPL_H_
#define WEBKIT_CHILD_WEBURLREQUEST_EXTRADATA_IMPL_H_

#include "base/compiler_specific.h"
#include "third_party/WebKit/public/platform/WebReferrerPolicy.h"
#include "third_party/WebKit/public/platform/WebString.h"
#include "third_party/WebKit/public/platform/WebURLRequest.h"
#include "webkit/child/webkit_child_export.h"

namespace webkit_glue {

class WEBKIT_CHILD_EXPORT WebURLRequestExtraDataImpl :
    public NON_EXPORTED_BASE(blink::WebURLRequest::ExtraData) {
 public:
  
  
  
  
  
  explicit WebURLRequestExtraDataImpl(
      blink::WebReferrerPolicy referrer_policy,
      const blink::WebString& custom_user_agent,
      bool was_after_preconnect_request);
  virtual ~WebURLRequestExtraDataImpl();

  blink::WebReferrerPolicy referrer_policy() const { return referrer_policy_; }
  const blink::WebString& custom_user_agent() const {
    return custom_user_agent_;
  }
  bool was_after_preconnect_request() { return was_after_preconnect_request_; }

 private:
  blink::WebReferrerPolicy referrer_policy_;
  blink::WebString custom_user_agent_;
  bool was_after_preconnect_request_;

  DISALLOW_COPY_AND_ASSIGN(WebURLRequestExtraDataImpl);
};

}  

#endif  
