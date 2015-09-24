/*
 * Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "native_client/src/include/elf32.h"
#include "native_client/src/include/elf_auxv.h"
#include "native_client/src/include/nacl_macros.h"
#include "native_client/src/untrusted/nacl/nacl_startup.h"
#include "ppapi/native_client/src/untrusted/pnacl_irt_shim/shim_ppapi.h"


void _pnacl_wrapper_start(uint32_t *info) {
  Elf32_auxv_t *auxv = nacl_startup_auxv(info);

  Elf32_auxv_t *entry = NULL;
  for (Elf32_auxv_t *av = auxv; av->a_type != AT_NULL; ++av) {
    if (av->a_type == AT_SYSINFO) {
      entry = av;
      break;
    }
  }

  if (entry != NULL) {
    __pnacl_real_irt_interface = (TYPE_nacl_irt_query) entry->a_un.a_val;

    entry->a_type = AT_SYSINFO;
    entry->a_un.a_val = (uintptr_t) __pnacl_irt_interface_wrapper;
  }


  _start(info);
}
