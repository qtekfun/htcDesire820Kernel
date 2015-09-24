/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_URL_UTIL_DEV_H_
#define PPAPI_C_DEV_PPB_URL_UTIL_DEV_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_URLUTIL_DEV_INTERFACE_0_6 "PPB_URLUtil(Dev);0.6"
#define PPB_URLUTIL_DEV_INTERFACE_0_7 "PPB_URLUtil(Dev);0.7"
#define PPB_URLUTIL_DEV_INTERFACE PPB_URLUTIL_DEV_INTERFACE_0_7



struct PP_URLComponent_Dev {
  int32_t begin;
  int32_t len;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_URLComponent_Dev, 8);

struct PP_URLComponents_Dev {
  struct PP_URLComponent_Dev scheme;
  struct PP_URLComponent_Dev username;
  struct PP_URLComponent_Dev password;
  struct PP_URLComponent_Dev host;
  struct PP_URLComponent_Dev port;
  struct PP_URLComponent_Dev path;
  struct PP_URLComponent_Dev query;
  struct PP_URLComponent_Dev ref;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_URLComponents_Dev, 64);

struct PPB_URLUtil_Dev_0_7 {
  struct PP_Var (*Canonicalize)(struct PP_Var url,
                                struct PP_URLComponents_Dev* components);
  struct PP_Var (*ResolveRelativeToURL)(
      struct PP_Var base_url,
      struct PP_Var relative_string,
      struct PP_URLComponents_Dev* components);
  struct PP_Var (*ResolveRelativeToDocument)(
      PP_Instance instance,
      struct PP_Var relative_string,
      struct PP_URLComponents_Dev* components);
  PP_Bool (*IsSameSecurityOrigin)(struct PP_Var url_a, struct PP_Var url_b);
  PP_Bool (*DocumentCanRequest)(PP_Instance instance, struct PP_Var url);
  PP_Bool (*DocumentCanAccessDocument)(PP_Instance active, PP_Instance target);
  struct PP_Var (*GetDocumentURL)(PP_Instance instance,
                                  struct PP_URLComponents_Dev* components);
  struct PP_Var (*GetPluginInstanceURL)(
      PP_Instance instance,
      struct PP_URLComponents_Dev* components);
  struct PP_Var (*GetPluginReferrerURL)(
      PP_Instance instance,
      struct PP_URLComponents_Dev* components);
};

typedef struct PPB_URLUtil_Dev_0_7 PPB_URLUtil_Dev;

struct PPB_URLUtil_Dev_0_6 {
  struct PP_Var (*Canonicalize)(struct PP_Var url,
                                struct PP_URLComponents_Dev* components);
  struct PP_Var (*ResolveRelativeToURL)(
      struct PP_Var base_url,
      struct PP_Var relative_string,
      struct PP_URLComponents_Dev* components);
  struct PP_Var (*ResolveRelativeToDocument)(
      PP_Instance instance,
      struct PP_Var relative_string,
      struct PP_URLComponents_Dev* components);
  PP_Bool (*IsSameSecurityOrigin)(struct PP_Var url_a, struct PP_Var url_b);
  PP_Bool (*DocumentCanRequest)(PP_Instance instance, struct PP_Var url);
  PP_Bool (*DocumentCanAccessDocument)(PP_Instance active, PP_Instance target);
  struct PP_Var (*GetDocumentURL)(PP_Instance instance,
                                  struct PP_URLComponents_Dev* components);
  struct PP_Var (*GetPluginInstanceURL)(
      PP_Instance instance,
      struct PP_URLComponents_Dev* components);
};

#endif  

