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

void *U(alloc)(U(descriptor) *desc, U(size_aau) n) {
#ifdef HMM_AUDIT_FAIL

  if (desc->avl_tree_root)
    AUDIT_BLOCK(PTR_REC_TO_HEAD(desc->avl_tree_root))
#endif

    if (desc->last_freed) {
#ifdef HMM_AUDIT_FAIL
      AUDIT_BLOCK(desc->last_freed)
#endif

      U(into_free_collection)(desc, (head_record *)(desc->last_freed));

      desc->last_freed = 0;
    }

  
  n += HEAD_AAUS;

  n = DIV_ROUND_UP(n, HMM_BLOCK_ALIGN_UNIT);

  if (n < MIN_BLOCK_BAUS)
    n = MIN_BLOCK_BAUS;

  {
    ptr_record *ptr_rec_ptr =
      U(avl_search)(
        (U(avl_avl) *) & (desc->avl_tree_root), (U(size_bau)) n,
        AVL_GREATER_EQUAL);

    return(ptr_rec_ptr ?
           U(alloc_from_bin)(desc, ptr_rec_ptr, (U(size_bau)) n) : 0);
  }
}
