// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_HOST_ERROR_CONVERSION_H_
#define PPAPI_HOST_ERROR_CONVERSION_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/host/ppapi_host_export.h"

namespace ppapi {
namespace host {

PPAPI_HOST_EXPORT int32_t NetErrorToPepperError(int net_error);

}  
}  

#endif  
