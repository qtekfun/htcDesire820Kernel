/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPP_H_
#define PPAPI_C_PPP_H_

#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/ppb.h"




#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/ppb.h"

#if __GNUC__ >= 4
#define PP_EXPORT __attribute__ ((visibility("default")))
#elif defined(_MSC_VER)
#define PP_EXPORT __declspec(dllexport)
#endif


#ifdef __cplusplus
extern "C" {
#endif


PP_EXPORT int32_t PPP_InitializeModule(PP_Module module,
                                       PPB_GetInterface get_browser_interface);


PP_EXPORT void PPP_ShutdownModule(void);


PP_EXPORT const void* PPP_GetInterface(const char* interface_name);

#ifdef __cplusplus
}  
#endif


typedef int32_t (*PP_InitializeModule_Func)(
    PP_Module module,
    PPB_GetInterface get_browser_interface);

typedef void (*PP_ShutdownModule_Func)(void);

typedef const void* (*PP_GetInterface_Func)(const char* interface_name);

#endif  

