/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_DEV_PPB_FILE_CHOOSER_DEV_H_
#define PPAPI_C_DEV_PPB_FILE_CHOOSER_DEV_H_

#include "ppapi/c/pp_array_output.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_FILECHOOSER_DEV_INTERFACE_0_5 "PPB_FileChooser(Dev);0.5"
#define PPB_FILECHOOSER_DEV_INTERFACE_0_6 "PPB_FileChooser(Dev);0.6"
#define PPB_FILECHOOSER_DEV_INTERFACE PPB_FILECHOOSER_DEV_INTERFACE_0_6



typedef enum {
  PP_FILECHOOSERMODE_OPEN = 0,
  PP_FILECHOOSERMODE_OPENMULTIPLE = 1
} PP_FileChooserMode_Dev;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_FileChooserMode_Dev, 4);

struct PPB_FileChooser_Dev_0_6 {
  PP_Resource (*Create)(PP_Instance instance,
                        PP_FileChooserMode_Dev mode,
                        struct PP_Var accept_types);
  PP_Bool (*IsFileChooser)(PP_Resource resource);
  int32_t (*Show)(PP_Resource chooser,
                  struct PP_ArrayOutput output,
                  struct PP_CompletionCallback callback);
};

typedef struct PPB_FileChooser_Dev_0_6 PPB_FileChooser_Dev;

struct PPB_FileChooser_Dev_0_5 {
  PP_Resource (*Create)(PP_Instance instance,
                        PP_FileChooserMode_Dev mode,
                        struct PP_Var accept_types);
  PP_Bool (*IsFileChooser)(PP_Resource resource);
  int32_t (*Show)(PP_Resource chooser, struct PP_CompletionCallback callback);
  PP_Resource (*GetNextChosenFile)(PP_Resource chooser);
};

#endif  

