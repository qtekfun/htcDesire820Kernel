/* Copyright (c) 2011 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef PPAPI_GENERATORS_PNACL_SHIM_H_
#define PPAPI_GENERATORS_PNACL_SHIM_H_

#include "ppapi/c/ppb.h"


typedef PPB_GetInterface PPP_GetInterface_Type;

void __set_real_Pnacl_PPBGetInterface(PPB_GetInterface real);
void __set_real_Pnacl_PPPGetInterface(PPP_GetInterface_Type real);

const void *__Pnacl_PPBGetInterface(const char *name);
const void *__Pnacl_PPPGetInterface(const char *name);

struct __PnaclWrapperInfo {
  const char* iface_macro;
  const void* wrapped_iface; 
  const void* real_iface;
};

#endif  
