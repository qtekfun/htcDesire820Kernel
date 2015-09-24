/* Copyright 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPB_TESTING_PRIVATE_H_
#define PPAPI_C_PRIVATE_PPB_TESTING_PRIVATE_H_

#include "ppapi/c/dev/ppb_url_util_dev.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_TESTING_PRIVATE_INTERFACE_1_0 "PPB_Testing_Private;1.0"
#define PPB_TESTING_PRIVATE_INTERFACE PPB_TESTING_PRIVATE_INTERFACE_1_0



struct PPB_Testing_Private_1_0 {
  PP_Bool (*ReadImageData)(PP_Resource device_context_2d,
                           PP_Resource image,
                           const struct PP_Point* top_left);
  void (*RunMessageLoop)(PP_Instance instance);
  void (*QuitMessageLoop)(PP_Instance instance);
  uint32_t (*GetLiveObjectsForInstance)(PP_Instance instance);
  PP_Bool (*IsOutOfProcess)(void);
  void (*SimulateInputEvent)(PP_Instance instance, PP_Resource input_event);
  struct PP_Var (*GetDocumentURL)(PP_Instance instance,
                                  struct PP_URLComponents_Dev* components);
  /**
   * Fetches up to |array_size| active PP_Vars in the tracker. Returns the
   * number of vars in the tracker. The active vars are written to |live_vars|
   * contiguously starting at index 0. The vars are not in any particular order.
   * If the number of live vars is greater than |array_size|, then an arbitrary
   * subset of |array_size| vars is written to |live_vars|. The reference count
   * of the returned PP_Vars will *not* be affected by this call.
   */
  uint32_t (*GetLiveVars)(struct PP_Var live_vars[], uint32_t array_size);
  void (*SetMinimumArrayBufferSizeForShmem)(PP_Instance instance,
                                            uint32_t threshold);
};

typedef struct PPB_Testing_Private_1_0 PPB_Testing_Private;

#endif  

