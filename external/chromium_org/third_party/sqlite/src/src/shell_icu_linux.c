/* Copyright 2007 Google Inc. All Rights Reserved.
**/

#include <limits.h>
#include <unistd.h>
#include "unicode/putil.h"
#include "unicode/udata.h"

int sqlite_shell_init_icu() {
  char bin_dir[PATH_MAX + 1];
  int bin_dir_size = readlink("/proc/self/exe", bin_dir, PATH_MAX);
  if (bin_dir_size < 0 || bin_dir_size > PATH_MAX)
    return 0;
  bin_dir[bin_dir_size] = 0;;

  u_setDataDirectory(bin_dir);
  
  
  UErrorCode err = U_ZERO_ERROR;
  udata_setFileAccess(UDATA_ONLY_PACKAGES, &err);
  return err == U_ZERO_ERROR;
}
