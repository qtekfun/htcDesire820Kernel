/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file. */

#ifndef LIBRARIES_NACL_IO_NACL_IO_H_
#define LIBRARIES_NACL_IO_NACL_IO_H_

#include <ppapi/c/pp_instance.h>
#include <ppapi/c/ppb.h>

#include "sdk_util/macros.h"

EXTERN_C_BEGIN

void nacl_io_init();

void nacl_io_init_ppapi(PP_Instance instance,
                        PPB_GetInterface get_interface);



struct fuse_operations;
int nacl_io_register_mount_type(const char* mount_type,
                                struct fuse_operations* fuse_ops);

int nacl_io_unregister_mount_type(const char* mount_type);

EXTERN_C_END

#endif  
