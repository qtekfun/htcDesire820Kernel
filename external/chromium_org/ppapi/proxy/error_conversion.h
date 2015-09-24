// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_ERROR_CONVERSION_H_
#define PPAPI_PROXY_ERROR_CONVERSION_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/proxy/ppapi_proxy_export.h"

namespace ppapi {
namespace proxy {

PPAPI_PROXY_EXPORT int32_t ConvertNetworkAPIErrorForCompatibility(
    int32_t pp_error,
    bool private_api);

}  
}  

#endif  
