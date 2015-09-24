/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */




#undef HMM_ADDR_ALIGN_UNIT
#undef HMM_BLOCK_ALIGN_UNIT
#undef HMM_UNIQUE
#undef HMM_DESC_PARAM
#undef HMM_SYM_TO_STRING
#undef HMM_SYM_TO_STRING
#undef HMM_AUDIT_FAIL

#define HMM_SYM_TO_STRING(X) HMM_SYM_TO_STRING(X)
#define HMM_SYM_TO_STRING(X) #X

#ifndef HMM_CNFG_NUM


#define HMM_UNIQUE(BASE) hmm_ ## BASE

#define HMM_ADDR_ALIGN_UNIT 32

#define HMM_BLOCK_ALIGN_UNIT 1

typedef unsigned long HMM_UNIQUE(size_aau);

typedef unsigned long HMM_UNIQUE(size_bau);

void hmm_dflt_abort(const char *, const char *);

#define HMM_AUDIT_FAIL \
  hmm_dflt_abort(__FILE__, HMM_SYM_TO_STRING(__LINE__));

#elif HMM_CNFG_NUM == 0


#define HMM_UNIQUE(BASE) thmm_ ## BASE

#define HMM_ADDR_ALIGN_UNIT sizeof(int)

#define HMM_BLOCK_ALIGN_UNIT 3

typedef unsigned HMM_UNIQUE(size_aau);

typedef unsigned short HMM_UNIQUE(size_bau);


extern jmp_buf HMM_UNIQUE(jmp_buf);
extern const char *HMM_UNIQUE(fail_file);
extern unsigned HMM_UNIQUE(fail_line);

#define HMM_AUDIT_FAIL \
  { HMM_UNIQUE(fail_file) = __FILE__; HMM_UNIQUE(fail_line) = __LINE__; \
    longjmp(HMM_UNIQUE(jmp_buf), 1); }

#elif HMM_CNFG_NUM == 1


#elif HMM_CNFG_NUM == 2


#elif HMM_CNFG_NUM == 3


#elif HMM_CNFG_NUM == 4


#elif HMM_CNFG_NUM == 5


#endif
