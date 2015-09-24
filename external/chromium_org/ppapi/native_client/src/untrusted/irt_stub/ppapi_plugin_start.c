/*
 * Copyright (c) 2011 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "native_client/src/include/elf32.h"
#include "native_client/src/include/elf_auxv.h"
#include "native_client/src/untrusted/irt/irt.h"
#include "native_client/src/untrusted/irt/irt_ppapi.h"
#include "ppapi/native_client/src/shared/ppapi_proxy/ppruntime.h"
#include "ppapi/native_client/src/untrusted/irt_stub/thread_creator.h"

static void fatal_error(const char *message) {
  write(2, message, strlen(message));
  _exit(127);
}

static Elf32_auxv_t *find_auxv(void) {
  char **ep = environ;
  while (*ep != NULL)
    ++ep;
  return (void *) (ep + 1);
}

static TYPE_nacl_irt_query grok_auxv(const Elf32_auxv_t *auxv) {
  const Elf32_auxv_t *av;
  for (av = auxv; av->a_type != AT_NULL; ++av) {
    if (av->a_type == AT_SYSINFO)
      return (TYPE_nacl_irt_query) av->a_un.a_val;
  }
  return NULL;
}

int PpapiPluginStart(const struct PP_StartFunctions *funcs) {
  TYPE_nacl_irt_query query_func = grok_auxv(find_auxv());

  if (NULL == query_func)
    fatal_error("PpapiPluginStart: No AT_SYSINFO item found in auxv, "
                "so cannot start PPAPI.  Is the IRT library not present?\n");

  struct nacl_irt_ppapihook hooks;
  if (sizeof(hooks) != query_func(NACL_IRT_PPAPIHOOK_v0_1,
                                  &hooks, sizeof(hooks)))
    fatal_error("PpapiPluginStart: PPAPI hooks not found\n");

  __nacl_register_thread_creator(&hooks);

  return hooks.ppapi_start(funcs);
}
