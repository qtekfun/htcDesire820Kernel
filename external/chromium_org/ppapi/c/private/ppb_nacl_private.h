/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_NACL_PRIVATE_H_
#define PPAPI_C_PRIVATE_PPB_NACL_PRIVATE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/private/ppb_instance_private.h"

#define PPB_NACL_PRIVATE_INTERFACE_1_0 "PPB_NaCl_Private;1.0"
#define PPB_NACL_PRIVATE_INTERFACE PPB_NACL_PRIVATE_INTERFACE_1_0



#include "ppapi/c/private/pp_file_handle.h"
#include "ppapi/c/private/ppb_instance_private.h"

typedef enum {
  PP_NACL_MANIFEST_MISSING_ARCH = 0
} PP_NaClError;

typedef enum {
  PP_NACL_EVENT_LOADSTART,
  PP_NACL_EVENT_PROGRESS,
  PP_NACL_EVENT_ERROR,
  PP_NACL_EVENT_ABORT,
  PP_NACL_EVENT_LOAD,
  PP_NACL_EVENT_LOADEND,
  PP_NACL_EVENT_CRASH
} PP_NaClEventType;

struct PPB_NaCl_Private_1_0 {
  PP_ExternalPluginResult (*LaunchSelLdr)(PP_Instance instance,
                                          const char* alleged_url,
                                          PP_Bool uses_irt,
                                          PP_Bool uses_ppapi,
                                          PP_Bool enable_ppapi_dev,
                                          PP_Bool enable_dyncode_syscalls,
                                          PP_Bool enable_exception_handling,
                                          PP_Bool enable_crash_throttling,
                                          void* imc_handle,
                                          struct PP_Var* error_message);
  PP_ExternalPluginResult (*StartPpapiProxy)(PP_Instance instance);
  int32_t (*UrandomFD)(void);
  PP_Bool (*Are3DInterfacesDisabled)(void);
  int32_t (*BrokerDuplicateHandle)(PP_FileHandle source_handle,
                                   uint32_t process_id,
                                   PP_FileHandle* target_handle,
                                   uint32_t desired_access,
                                   uint32_t options);
  PP_FileHandle (*GetReadonlyPnaclFd)(const char* filename);
  PP_FileHandle (*CreateTemporaryFile)(PP_Instance instance);
  int32_t (*GetNexeFd)(PP_Instance instance,
                       const char* pexe_url,
                       uint32_t abi_version,
                       uint32_t opt_level,
                       const char* last_modified,
                       const char* etag,
                       PP_Bool has_no_store_header,
                       PP_Bool* is_hit,
                       PP_FileHandle* nexe_handle,
                       struct PP_CompletionCallback callback);
  void (*ReportTranslationFinished)(PP_Instance instance, PP_Bool success);
  
  PP_ExternalPluginResult (*ReportNaClError)(PP_Instance instance,
                                             PP_NaClError message_id);
  PP_FileHandle (*OpenNaClExecutable)(PP_Instance instance,
                                      const char* file_url,
                                      uint64_t* file_token_lo,
                                      uint64_t* file_token_hi);
  void (*DispatchEvent)(PP_Instance instance,
                        PP_NaClEventType event_type,
                        struct PP_Var resource_url,
                        PP_Bool length_is_computable,
                        uint64_t loaded_bytes,
                        uint64_t total_bytes);
  void (*SetReadOnlyProperty)(PP_Instance instance,
                              struct PP_Var key,
                              struct PP_Var value);
};

typedef struct PPB_NaCl_Private_1_0 PPB_NaCl_Private;

#endif  

