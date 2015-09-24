/*
 * Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "ppapi/native_client/src/untrusted/pnacl_irt_shim/shim_ppapi.h"

#include <string.h>
#include "native_client/src/include/nacl_macros.h"
#include "native_client/src/untrusted/irt/irt.h"
#include "native_client/src/untrusted/irt/irt_dev.h"
#include "native_client/src/untrusted/irt/irt_ppapi.h"
#include "ppapi/generators/pnacl_shim.h"
#include "ppapi/native_client/src/shared/ppapi_proxy/ppruntime.h"


static const char *const irt_interface_whitelist[] = {
  NACL_IRT_BASIC_v0_1,
  NACL_IRT_MEMORY_v0_3,
  NACL_IRT_THREAD_v0_1,
  NACL_IRT_FUTEX_v0_1,
  NACL_IRT_TLS_v0_1,
  NACL_IRT_PPAPIHOOK_v0_1,
  NACL_IRT_RANDOM_v0_1,
  NACL_IRT_CLOCK_v0_1,
  
  NACL_IRT_DEV_FDIO_v0_1,
  NACL_IRT_DEV_FILENAME_v0_2,
};

static int mystrcmp(const char* s1, const char *s2) {
  while((*s1 && *s2) && (*s1++ == *s2++));
  return *(--s1) - *(--s2);
}

static int is_irt_interface_whitelisted(const char *interface_name) {
  int i;
  for (i = 0; i < NACL_ARRAY_SIZE(irt_interface_whitelist); i++) {
    if (mystrcmp(interface_name, irt_interface_whitelist[i]) == 0) {
      return 1;
    }
  }
  return 0;
}

TYPE_nacl_irt_query __pnacl_real_irt_interface;

static struct PP_StartFunctions user_start_functions;

static int32_t wrap_PPPInitializeModule(PP_Module module_id,
                                        PPB_GetInterface get_browser_intf) {
  __set_real_Pnacl_PPBGetInterface(get_browser_intf);
  return (*user_start_functions.PPP_InitializeModule)(module_id,
                                                      &__Pnacl_PPBGetInterface);
}

static void wrap_PPPShutdownModule() {
  (*user_start_functions.PPP_ShutdownModule)();
}

static const struct PP_StartFunctions wrapped_ppapi_methods = {
  wrap_PPPInitializeModule,
  wrap_PPPShutdownModule,
  __Pnacl_PPPGetInterface
};

static struct nacl_irt_ppapihook real_irt_ppapi_hook;

static int wrap_ppapi_start(const struct PP_StartFunctions *funcs) {
  user_start_functions = *funcs;
  __set_real_Pnacl_PPPGetInterface(user_start_functions.PPP_GetInterface);

  return (*real_irt_ppapi_hook.ppapi_start)(&wrapped_ppapi_methods);
}

size_t __pnacl_irt_interface_wrapper(const char *interface_ident,
                                     void *table, size_t tablesize) {
  if (!is_irt_interface_whitelisted(interface_ident))
    return 0;

  if (0 != mystrcmp(interface_ident, NACL_IRT_PPAPIHOOK_v0_1)) {
    return (*__pnacl_real_irt_interface)(interface_ident, table, tablesize);
  }
  if ((*__pnacl_real_irt_interface)(NACL_IRT_PPAPIHOOK_v0_1,
                                    &real_irt_ppapi_hook,
                                    sizeof real_irt_ppapi_hook) !=
      sizeof real_irt_ppapi_hook) {
    return 0;
  }
  struct nacl_irt_ppapihook *dest = table;
  if (sizeof *dest <= tablesize) {
    dest->ppapi_start = wrap_ppapi_start;
    dest->ppapi_register_thread_creator =
        real_irt_ppapi_hook.ppapi_register_thread_creator;
    return sizeof *dest;
  }
  return 0;
}
