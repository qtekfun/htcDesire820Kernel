// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_URL_REQUEST_INFO_UTIL_H_
#define CONTENT_RENDERER_PEPPER_URL_REQUEST_INFO_UTIL_H_

#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"
#include "ppapi/c/pp_instance.h"

namespace ppapi {
struct URLRequestInfoData;
}

namespace blink {
class WebFrame;
class WebURLRequest;
}

namespace content {

CONTENT_EXPORT bool CreateWebURLRequest(PP_Instance instance,
                                        ppapi::URLRequestInfoData* data,
                                        blink::WebFrame* frame,
                                        blink::WebURLRequest* dest);

CONTENT_EXPORT bool URLRequestRequiresUniversalAccess(
    const ppapi::URLRequestInfoData& data);

}  

#endif  
