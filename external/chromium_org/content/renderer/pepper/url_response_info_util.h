// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_URL_RESPONSE_INFO_UTIL_H_
#define CONTENT_RENDERER_PEPPER_URL_RESPONSE_INFO_UTIL_H_

#include "base/callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/shared_impl/url_response_info_data.h"

namespace blink {
class WebURLResponse;
}

namespace content {
class RendererPpapiHostImpl;

typedef base::Callback<void(const ppapi::URLResponseInfoData&)>
    DataFromWebURLResponseCallback;

void DataFromWebURLResponse(RendererPpapiHostImpl* host_impl,
                            PP_Instance pp_instance,
                            const blink::WebURLResponse& response,
                            const DataFromWebURLResponseCallback& callback);

}  

#endif  
