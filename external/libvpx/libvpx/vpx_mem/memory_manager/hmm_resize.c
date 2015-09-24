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

int U(resize)(U(descriptor) *desc, void *mem, U(size_aau) n) {
  U(size_aau) i;
  head_record *next_head_ptr;
  head_record *head_ptr = PTR_REC_TO_HEAD(mem);

  
  int next_block_free;

  
  n += HEAD_AAUS;
  n = DIV_ROUND_UP(n, HMM_BLOCK_ALIGN_UNIT);

  if (n < MIN_BLOCK_BAUS)
    n = MIN_BLOCK_BAUS;

#ifdef HMM_AUDIT_FAIL

  AUDIT_BLOCK(head_ptr)

  if (!IS_BLOCK_ALLOCATED(head_ptr))
    HMM_AUDIT_FAIL

    if (desc->avl_tree_root)
      AUDIT_BLOCK(PTR_REC_TO_HEAD(desc->avl_tree_root))

#endif

      i = head_ptr->block_size;

  next_head_ptr =
    (head_record *) BAUS_FORWARD(head_ptr, head_ptr->block_size);

  next_block_free =
    (next_head_ptr == desc->last_freed) ||
    !IS_BLOCK_ALLOCATED(next_head_ptr);

  if (next_block_free)
    
    i += BLOCK_BAUS(next_head_ptr);

  if (n > i)
    
    return(-1);

  if (next_block_free) {
#ifdef HMM_AUDIT_FAIL
    AUDIT_BLOCK(next_head_ptr)
#endif

    if (next_head_ptr == desc->last_freed)
      desc->last_freed = 0;
    else
      U(out_of_free_collection)(desc, next_head_ptr);

    next_head_ptr =
      (head_record *) BAUS_FORWARD(head_ptr, (U(size_bau)) i);
  }

  
  i -= n;

  if (i < MIN_BLOCK_BAUS)
  {
    n += i;
    i = n;
  } else {

    head_record *rem_head_ptr;

    rem_head_ptr = (head_record *) BAUS_FORWARD(head_ptr, n);

    rem_head_ptr->previous_block_size = (U(size_bau)) n;
    rem_head_ptr->block_size = (U(size_bau)) i;

    if (desc->last_freed) {
#ifdef HMM_AUDIT_FAIL
      AUDIT_BLOCK(desc->last_freed)
#endif

      U(into_free_collection)(desc, (head_record *)(desc->last_freed));

      desc->last_freed = 0;
    }

    desc->last_freed = rem_head_ptr;
  }

  head_ptr->block_size = (U(size_bau)) n;
  next_head_ptr->previous_block_size = (U(size_bau)) i;

  return(0);
}
