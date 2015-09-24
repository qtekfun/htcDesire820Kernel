// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ENCODINGS_COMPACT_LANG_DET_WIN_CLD_UTF8STATETABLE_H_
#define ENCODINGS_COMPACT_LANG_DET_WIN_CLD_UTF8STATETABLE_H_

#if !defined(CLD_WINDOWS)

#include "util/utf8/utf8statetable.h"

#else

#include "encodings/compact_lang_det/win/cld_basictypes.h"

struct RemapEntry {
  uint8 delete_bytes;
  uint8 add_bytes;
  uint16 bytes_offset;
};

typedef enum {
  kExitDstSpaceFull = 239,
  kExitIllegalStructure,  
  kExitOK,                
  kExitReject,            
  kExitReplace1,
  kExitReplace2,
  kExitReplace3,
  kExitReplace21,
  kExitReplace31,
  kExitReplace32,
  kExitReplaceOffset1,
  kExitReplaceOffset2,
  kExitReplace1S0,
  kExitSpecial,
  kExitDoAgain,
  kExitRejectAlt,
  kExitNone               
} ExitReason;

typedef enum {
  kExitDstSpaceFull_2 = -32769,
  kExitIllegalStructure_2,  
  kExitOK_2,                
  kExitReject_2,            
  kExitReplace1_2,
  kExitReplace2_2,
  kExitReplace3_2,
  kExitReplace21_2,
  kExitReplace31_2,
  kExitReplace32_2,
  kExitReplaceOffset1_2,
  kExitReplaceOffset2_2,
  kExitReplace1S0_2,
  kExitSpecial_2,
  kExitDoAgain_2,
  kExitRejectAlt_2,
  kExitNone_2               
} ExitReason_2;

typedef struct {
  const uint32 state0;
  const uint32 state0_size;
  const uint32 total_size;
  const int max_expand;
  const int entry_shift;
  const int bytes_per_entry;
  const uint32 losub;
  const uint32 hiadd;
  const uint8* state_table;
  const RemapEntry* remap_base;
  const uint8* remap_string;
  const uint8* fast_state;
} UTF8StateMachineObj;

typedef struct {
  const uint32 state0;
  const uint32 state0_size;
  const uint32 total_size;
  const int max_expand;
  const int entry_shift;
  const int bytes_per_entry;
  const uint32 losub;
  const uint32 hiadd;
  const signed short* state_table;
  const RemapEntry* remap_base;
  const uint8* remap_string;
  const uint8* fast_state;
} UTF8StateMachineObj_2;


typedef UTF8StateMachineObj UTF8PropObj;
typedef UTF8StateMachineObj UTF8ScanObj;
typedef UTF8StateMachineObj_2 UTF8PropObj_2;


uint8 UTF8GenericProperty(const UTF8PropObj* st,
                          const uint8** src,
                          int* srclen);


uint8 UTF8GenericPropertyBigOneByte(const UTF8PropObj* st,
                          const uint8** src,
                          int* srclen);

int UTF8GenericScan(const UTF8ScanObj* st,
                    const uint8* str,
                    const int len,
                    int* bytes_consumed);

#endif

#endif  
