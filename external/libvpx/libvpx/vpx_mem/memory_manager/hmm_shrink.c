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

void U(shrink_chunk)(U(descriptor) *desc, U(size_bau) n_baus_to_shrink) {
  head_record *dummy_end_block = (head_record *)
                                 BAUS_BACKWARD(desc->end_of_shrinkable_chunk, DUMMY_END_BLOCK_BAUS);

#ifdef HMM_AUDIT_FAIL

  if (dummy_end_block->block_size != 0)
    
    HMM_AUDIT_FAIL

#endif

    if (n_baus_to_shrink) {
      head_record *last_block = (head_record *)
                                BAUS_BACKWARD(
                                  dummy_end_block, dummy_end_block->previous_block_size);

#ifdef HMM_AUDIT_FAIL
      AUDIT_BLOCK(last_block)
#endif

      if (last_block == desc->last_freed) {
        U(size_bau) bs = BLOCK_BAUS(last_block);

        int chunk_will_survive =
          (PREV_BLOCK_BAUS(last_block) && (n_baus_to_shrink == bs)) ||
          (n_baus_to_shrink <= (U(size_bau))(bs - MIN_BLOCK_BAUS));

        if (chunk_will_survive ||
            (!PREV_BLOCK_BAUS(last_block) &&
             (n_baus_to_shrink ==
              (U(size_bau))(bs + DUMMY_END_BLOCK_BAUS)))) {
          desc->last_freed = 0;

          if (chunk_will_survive) {
            bs -= n_baus_to_shrink;

            if (bs) {

              last_block->block_size = bs;

              dummy_end_block =
                (head_record *) BAUS_FORWARD(last_block, bs);
              dummy_end_block->previous_block_size = bs;
              dummy_end_block->block_size = 0;

#ifdef HMM_AUDIT_FAIL

              if (desc->avl_tree_root)
                AUDIT_BLOCK(PTR_REC_TO_HEAD(desc->avl_tree_root))
#endif

                U(into_free_collection)(desc, last_block);
            } else {
              last_block->block_size = 0;
              last_block->previous_block_size &= ~HIGH_BIT_BAU_SIZE;
            }
          }
        }

#ifdef HMM_AUDIT_FAIL
        else
          HMM_AUDIT_FAIL
#endif
        }

#ifdef HMM_AUDIT_FAIL
      else
        HMM_AUDIT_FAIL
#endif
      }
}
