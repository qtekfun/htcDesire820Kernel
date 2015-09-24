/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */




#undef HMM_PROCESS


#ifndef HMM_CNFG_NUM


#ifndef HMM_INC_CNFG_DFLT
#define HMM_INC_CNFG_DFLT
#define HMM_PROCESS
#endif

#elif HMM_CNFG_NUM == 0


#ifndef HMM_INC_CNFG_0
#define HMM_INC_CNFG_0
#define HMM_PROCESS
#endif

#elif HMM_CNFG_NUM == 1

#ifndef HMM_INC_CNFG_1
#define HMM_INC_CNFG_1
#define HMM_PROCESS
#endif

#elif HMM_CNFG_NUM == 2

#ifndef HMM_INC_CNFG_2
#define HMM_INC_CNFG_2
#define HMM_PROCESS
#endif

#elif HMM_CNFG_NUM == 3

#ifndef HMM_INC_CNFG_3
#define HMM_INC_CNFG_3
#define HMM_PROCESS
#endif

#elif HMM_CNFG_NUM == 4

#ifndef HMM_INC_CNFG_4
#define HMM_INC_CNFG_4
#define HMM_PROCESS
#endif

#elif HMM_CNFG_NUM == 5

#ifndef HMM_INC_CNFG_5
#define HMM_INC_CNFG_5
#define HMM_PROCESS
#endif

#endif

#ifdef HMM_PROCESS

#include "hmm_cnfg.h"

typedef struct HMM_UNIQUE(structure) {
  

  void *avl_tree_root;

  
  void *last_freed;

  

  HMM_UNIQUE(size_bau) num_baus_can_shrink;
  void *end_of_shrinkable_chunk;
}
HMM_UNIQUE(descriptor);


void HMM_UNIQUE(init)(HMM_UNIQUE(descriptor) *desc);

void *HMM_UNIQUE(alloc)(
  HMM_UNIQUE(descriptor) *desc, HMM_UNIQUE(size_aau) num_addr_align_units);

void *HMM_UNIQUE(greedy_alloc)(
  HMM_UNIQUE(descriptor) *desc, HMM_UNIQUE(size_aau) needed_addr_align_units,
  HMM_UNIQUE(size_aau) coveted_addr_align_units);

int HMM_UNIQUE(resize)(
  HMM_UNIQUE(descriptor) *desc, void *mem,
  HMM_UNIQUE(size_aau) num_addr_align_units);

int HMM_UNIQUE(greedy_resize)(
  HMM_UNIQUE(descriptor) *desc, void *mem,
  HMM_UNIQUE(size_aau) needed_addr_align_units,
  HMM_UNIQUE(size_aau) coveted_addr_align_units);

void HMM_UNIQUE(free)(HMM_UNIQUE(descriptor) *desc, void *mem);

HMM_UNIQUE(size_aau) HMM_UNIQUE(true_size)(void *mem);

HMM_UNIQUE(size_aau) HMM_UNIQUE(largest_available)(
  HMM_UNIQUE(descriptor) *desc);

void HMM_UNIQUE(new_chunk)(
  HMM_UNIQUE(descriptor) *desc, void *start_of_chunk,
  HMM_UNIQUE(size_bau) num_block_align_units);

void HMM_UNIQUE(grow_chunk)(
  HMM_UNIQUE(descriptor) *desc, void *end_of_chunk,
  HMM_UNIQUE(size_bau) num_block_align_units);

void HMM_UNIQUE(shrink_chunk)(
  HMM_UNIQUE(descriptor) *desc,
  HMM_UNIQUE(size_bau) num_block_align_units);

#endif 
