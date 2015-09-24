/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_URL_REQUEST_INFO_H_
#define PPAPI_C_PPB_URL_REQUEST_INFO_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_time.h"
#include "ppapi/c/pp_var.h"

#define PPB_URLREQUESTINFO_INTERFACE_1_0 "PPB_URLRequestInfo;1.0"
#define PPB_URLREQUESTINFO_INTERFACE PPB_URLREQUESTINFO_INTERFACE_1_0



typedef enum {
  
  PP_URLREQUESTPROPERTY_URL = 0,
  PP_URLREQUESTPROPERTY_METHOD = 1,
  PP_URLREQUESTPROPERTY_HEADERS = 2,
  PP_URLREQUESTPROPERTY_STREAMTOFILE = 3,
  PP_URLREQUESTPROPERTY_FOLLOWREDIRECTS = 4,
  PP_URLREQUESTPROPERTY_RECORDDOWNLOADPROGRESS = 5,
  PP_URLREQUESTPROPERTY_RECORDUPLOADPROGRESS = 6,
  PP_URLREQUESTPROPERTY_CUSTOMREFERRERURL = 7,
  PP_URLREQUESTPROPERTY_ALLOWCROSSORIGINREQUESTS = 8,
  PP_URLREQUESTPROPERTY_ALLOWCREDENTIALS = 9,
  PP_URLREQUESTPROPERTY_CUSTOMCONTENTTRANSFERENCODING = 10,
  PP_URLREQUESTPROPERTY_PREFETCHBUFFERUPPERTHRESHOLD = 11,
  PP_URLREQUESTPROPERTY_PREFETCHBUFFERLOWERTHRESHOLD = 12,
  PP_URLREQUESTPROPERTY_CUSTOMUSERAGENT = 13
} PP_URLRequestProperty;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_URLRequestProperty, 4);

struct PPB_URLRequestInfo_1_0 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsURLRequestInfo)(PP_Resource resource);
  PP_Bool (*SetProperty)(PP_Resource request,
                         PP_URLRequestProperty property,
                         struct PP_Var value);
  PP_Bool (*AppendDataToBody)(PP_Resource request,
                              const void* data,
                              uint32_t len);
  PP_Bool (*AppendFileToBody)(PP_Resource request,
                              PP_Resource file_ref,
                              int64_t start_offset,
                              int64_t number_of_bytes,
                              PP_Time expected_last_modified_time);
};

typedef struct PPB_URLRequestInfo_1_0 PPB_URLRequestInfo;

#endif  

