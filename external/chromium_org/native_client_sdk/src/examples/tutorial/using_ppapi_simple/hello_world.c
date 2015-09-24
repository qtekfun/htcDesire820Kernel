/* Copyright (c) 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <stdio.h>
#include <string.h>

#include "ppapi_simple/ps_main.h"

#ifdef SEL_LDR
#define example_main main
#endif

int example_main(int argc, char* argv[]) {
  
  printf("Hello World STDOUT.\n");

  
  fprintf(stderr, "Hello World STDERR.\n");
  return 0;
}

PPAPI_SIMPLE_REGISTER_MAIN(example_main)
