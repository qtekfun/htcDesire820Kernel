/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */



#include "hmm_intrnl.h"
#include <stdio.h>
#include <stdlib.h>

static int entered = 0;

void hmm_dflt_abort(const char *file, const char *line) {
  

  if (entered)

    while (1);

  entered = 1;

  fputs("\n_abort - Heap corruption\n" "File: ", stderr);
  fputs(file, stderr);
  fputs("  Line: ", stderr);
  fputs(line, stderr);
  fputs("\n\n", stderr);
  fputs("hmm_dflt_abort: while(1)!!!\n", stderr);
  fflush(stderr);

  while (1);
}
