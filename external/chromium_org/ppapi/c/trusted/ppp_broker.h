/* Copyright (c) 2011 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_TRUSTED_PPP_BROKER_H_
#define PPAPI_C_TRUSTED_PPP_BROKER_H_

#include "ppapi/c/pp_macros.h"




#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_stdint.h"


#if __GNUC__ >= 4

#define PP_EXPORT __attribute__ ((visibility("default")))
#elif defined(_MSC_VER)
#define PP_EXPORT __declspec(dllexport)
#endif



#ifdef __cplusplus
extern "C" {
#endif


typedef int32_t (*PP_ConnectInstance_Func)(PP_Instance instance,
                                           int32_t handle);


PP_EXPORT int32_t PPP_InitializeBroker(
    PP_ConnectInstance_Func* connect_instance_func);


PP_EXPORT void PPP_ShutdownBroker();

#ifdef __cplusplus
}  
#endif

#endif  

