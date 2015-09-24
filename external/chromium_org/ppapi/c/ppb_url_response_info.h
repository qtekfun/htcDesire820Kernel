/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_URL_RESPONSE_INFO_H_
#define PPAPI_C_PPB_URL_RESPONSE_INFO_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_URLRESPONSEINFO_INTERFACE_1_0 "PPB_URLResponseInfo;1.0"
#define PPB_URLRESPONSEINFO_INTERFACE PPB_URLRESPONSEINFO_INTERFACE_1_0



typedef enum {
  PP_URLRESPONSEPROPERTY_URL = 0,
  PP_URLRESPONSEPROPERTY_REDIRECTURL = 1,
  PP_URLRESPONSEPROPERTY_REDIRECTMETHOD = 2,
  PP_URLRESPONSEPROPERTY_STATUSCODE = 3,
  PP_URLRESPONSEPROPERTY_STATUSLINE = 4,
  PP_URLRESPONSEPROPERTY_HEADERS = 5
} PP_URLResponseProperty;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_URLResponseProperty, 4);

struct PPB_URLResponseInfo_1_0 {
  PP_Bool (*IsURLResponseInfo)(PP_Resource resource);
  struct PP_Var (*GetProperty)(PP_Resource response,
                               PP_URLResponseProperty property);
  PP_Resource (*GetBodyAsFileRef)(PP_Resource response);
};

typedef struct PPB_URLResponseInfo_1_0 PPB_URLResponseInfo;

#endif  

