/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_FILE_REF_PRIVATE_H_
#define PPAPI_C_PRIVATE_PPB_FILE_REF_PRIVATE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_FILEREFPRIVATE_INTERFACE_0_1 "PPB_FileRefPrivate;0.1"
#define PPB_FILEREFPRIVATE_INTERFACE PPB_FILEREFPRIVATE_INTERFACE_0_1



struct PPB_FileRefPrivate_0_1 {
  struct PP_Var (*GetAbsolutePath)(PP_Resource file_ref);
};

typedef struct PPB_FileRefPrivate_0_1 PPB_FileRefPrivate;

#endif  

