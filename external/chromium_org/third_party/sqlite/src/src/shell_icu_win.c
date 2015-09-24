/* Copyright 2011 Google Inc. All Rights Reserved.
**/

#include <windows.h>
#include "unicode/udata.h"


#define ICU_DATA_SYMBOL "icudt" U_ICU_VERSION_SHORT "_dat"
int sqlite_shell_init_icu() {
  HMODULE module;
  FARPROC addr;
  UErrorCode err;

  
  module = LoadLibrary(L"icudt.dll");
  if (!module)
    return 0;

  addr = GetProcAddress(module, ICU_DATA_SYMBOL);
  if (!addr)
    return 0;

  err = U_ZERO_ERROR;
  udata_setCommonData(addr, &err);

  return 1;
}
