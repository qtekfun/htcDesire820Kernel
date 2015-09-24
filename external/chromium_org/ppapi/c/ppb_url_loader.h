/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_URL_LOADER_H_
#define PPAPI_C_PPB_URL_LOADER_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_URLLOADER_INTERFACE_1_0 "PPB_URLLoader;1.0"
#define PPB_URLLOADER_INTERFACE PPB_URLLOADER_INTERFACE_1_0



struct PPB_URLLoader_1_0 {
  PP_Resource (*Create)(PP_Instance instance);
  PP_Bool (*IsURLLoader)(PP_Resource resource);
  int32_t (*Open)(PP_Resource loader,
                  PP_Resource request_info,
                  struct PP_CompletionCallback callback);
  int32_t (*FollowRedirect)(PP_Resource loader,
                            struct PP_CompletionCallback callback);
  PP_Bool (*GetUploadProgress)(PP_Resource loader,
                               int64_t* bytes_sent,
                               int64_t* total_bytes_to_be_sent);
  PP_Bool (*GetDownloadProgress)(PP_Resource loader,
                                 int64_t* bytes_received,
                                 int64_t* total_bytes_to_be_received);
  PP_Resource (*GetResponseInfo)(PP_Resource loader);
  int32_t (*ReadResponseBody)(PP_Resource loader,
                              void* buffer,
                              int32_t bytes_to_read,
                              struct PP_CompletionCallback callback);
  int32_t (*FinishStreamingToFile)(PP_Resource loader,
                                   struct PP_CompletionCallback callback);
  void (*Close)(PP_Resource loader);
};

typedef struct PPB_URLLoader_1_0 PPB_URLLoader;

#endif  

