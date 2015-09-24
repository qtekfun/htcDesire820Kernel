/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_TRUSTED_PPB_FILE_CHOOSER_TRUSTED_H_
#define PPAPI_C_TRUSTED_PPB_FILE_CHOOSER_TRUSTED_H_

#include "ppapi/c/pp_array_output.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_FILECHOOSER_TRUSTED_INTERFACE_0_5 "PPB_FileChooserTrusted;0.5"
#define PPB_FILECHOOSER_TRUSTED_INTERFACE_0_6 "PPB_FileChooserTrusted;0.6"
#define PPB_FILECHOOSER_TRUSTED_INTERFACE PPB_FILECHOOSER_TRUSTED_INTERFACE_0_6



struct PPB_FileChooserTrusted_0_6 {
  int32_t (*ShowWithoutUserGesture)(PP_Resource chooser,
                                    PP_Bool save_as,
                                    struct PP_Var suggested_file_name,
                                    struct PP_ArrayOutput output,
                                    struct PP_CompletionCallback callback);
};

typedef struct PPB_FileChooserTrusted_0_6 PPB_FileChooserTrusted;

struct PPB_FileChooserTrusted_0_5 {
  int32_t (*ShowWithoutUserGesture)(PP_Resource chooser,
                                    PP_Bool save_as,
                                    struct PP_Var suggested_file_name,
                                    struct PP_CompletionCallback callback);
};

#endif  

