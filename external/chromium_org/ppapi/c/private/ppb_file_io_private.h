/* Copyright (c) 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_FILE_IO_PRIVATE_H_
#define PPAPI_C_PRIVATE_PPB_FILE_IO_PRIVATE_H_

#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_FILEIO_PRIVATE_INTERFACE_0_1 "PPB_FileIO_Private;0.1"
#define PPB_FILEIO_PRIVATE_INTERFACE PPB_FILEIO_PRIVATE_INTERFACE_0_1



#include "ppapi/c/private/pp_file_handle.h"

struct PPB_FileIO_Private_0_1 {
  int32_t (*RequestOSFileHandle)(PP_Resource file_io,
                                 PP_FileHandle* handle,
                                 struct PP_CompletionCallback callback);
};

typedef struct PPB_FileIO_Private_0_1 PPB_FileIO_Private;

#endif  

