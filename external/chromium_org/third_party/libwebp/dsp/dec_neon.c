// Copyright 2012 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#include "./dsp.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if defined(WEBP_USE_NEON)

#include "../dec/vp8i.h"

#define QRegs "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7",                  \
              "q8", "q9", "q10", "q11", "q12", "q13", "q14", "q15"

#define FLIP_SIGN_BIT2(a, b, s)                                                \
  "veor     " #a "," #a "," #s "               \n"                             \
  "veor     " #b "," #b "," #s "               \n"                             \

#define FLIP_SIGN_BIT4(a, b, c, d, s)                                          \
  FLIP_SIGN_BIT2(a, b, s)                                                      \
  FLIP_SIGN_BIT2(c, d, s)                                                      \

#define NEEDS_FILTER(p1, p0, q0, q1, thresh, mask)                             \
  "vabd.u8    q15," #p0 "," #q0 "         \n"                \
  "vabd.u8    q14," #p1 "," #q1 "         \n"                \
  "vqadd.u8   q15, q15, q15               \n"            \
  "vshr.u8    q14, q14, #1                \n"            \
  "vqadd.u8   q15, q15, q14     \n"   \
  "vdup.8     q14, " #thresh "            \n"                                  \
  "vcge.u8   " #mask ", q14, q15          \n"  

#define GET_BASE_DELTA(p1, p0, q0, q1, o)                                      \
  "vqsub.s8   q15," #q0 "," #p0 "         \n"                   \
  "vqsub.s8  " #o "," #p1 "," #q1 "       \n"                   \
  "vqadd.s8  " #o "," #o ", q15           \n"   \
  "vqadd.s8  " #o "," #o ", q15           \n"   \
  "vqadd.s8  " #o "," #o ", q15           \n"  

#define DO_SIMPLE_FILTER(p0, q0, fl)                                           \
  "vmov.i8    q15, #0x03                  \n"                                  \
  "vqadd.s8   q15, q15, " #fl "           \n"        \
  "vshr.s8    q15, q15, #3                \n"                \
  "vqadd.s8  " #p0 "," #p0 ", q15         \n"               \
                                                                               \
  "vmov.i8    q15, #0x04                  \n"                                  \
  "vqadd.s8   q15, q15, " #fl "           \n"        \
  "vshr.s8    q15, q15, #3                \n"                \
  "vqsub.s8  " #q0 "," #q0 ", q15         \n"  

#define DO_FILTER2(p1, p0, q0, q1, thresh)                                     \
  NEEDS_FILTER(p1, p0, q0, q1, thresh, q9)              \
  "vmov.i8    q10, #0x80                  \n"                    \
  FLIP_SIGN_BIT4(p1, p0, q0, q1, q10)             \
  GET_BASE_DELTA(p1, p0, q0, q1, q11)                   \
  "vand       q9, q9, q11                 \n"           \
  DO_SIMPLE_FILTER(p0, q0, q9)                               \
  FLIP_SIGN_BIT2(p0, q0, q10)

#define LOAD8x4(c1, c2, c3, c4, b1, b2, stride)                                \
  "vld4.8   {" #c1"[0], " #c2"[0], " #c3"[0], " #c4"[0]}," #b1 "," #stride"\n" \
  "vld4.8   {" #c1"[1], " #c2"[1], " #c3"[1], " #c4"[1]}," #b2 "," #stride"\n" \
  "vld4.8   {" #c1"[2], " #c2"[2], " #c3"[2], " #c4"[2]}," #b1 "," #stride"\n" \
  "vld4.8   {" #c1"[3], " #c2"[3], " #c3"[3], " #c4"[3]}," #b2 "," #stride"\n" \
  "vld4.8   {" #c1"[4], " #c2"[4], " #c3"[4], " #c4"[4]}," #b1 "," #stride"\n" \
  "vld4.8   {" #c1"[5], " #c2"[5], " #c3"[5], " #c4"[5]}," #b2 "," #stride"\n" \
  "vld4.8   {" #c1"[6], " #c2"[6], " #c3"[6], " #c4"[6]}," #b1 "," #stride"\n" \
  "vld4.8   {" #c1"[7], " #c2"[7], " #c3"[7], " #c4"[7]}," #b2 "," #stride"\n"

#define STORE8x2(c1, c2, p, stride)                                            \
  "vst2.8   {" #c1"[0], " #c2"[0]}," #p "," #stride " \n"                      \
  "vst2.8   {" #c1"[1], " #c2"[1]}," #p "," #stride " \n"                      \
  "vst2.8   {" #c1"[2], " #c2"[2]}," #p "," #stride " \n"                      \
  "vst2.8   {" #c1"[3], " #c2"[3]}," #p "," #stride " \n"                      \
  "vst2.8   {" #c1"[4], " #c2"[4]}," #p "," #stride " \n"                      \
  "vst2.8   {" #c1"[5], " #c2"[5]}," #p "," #stride " \n"                      \
  "vst2.8   {" #c1"[6], " #c2"[6]}," #p "," #stride " \n"                      \
  "vst2.8   {" #c1"[7], " #c2"[7]}," #p "," #stride " \n"


static void SimpleVFilter16NEON(uint8_t* p, int stride, int thresh) {
  __asm__ volatile (
    "sub        %[p], %[p], %[stride], lsl #1  \n"  

    "vld1.u8    {q1}, [%[p]], %[stride]        \n"  
    "vld1.u8    {q2}, [%[p]], %[stride]        \n"  
    "vld1.u8    {q3}, [%[p]], %[stride]        \n"  
    "vld1.u8    {q4}, [%[p]]                   \n"  

    DO_FILTER2(q1, q2, q3, q4, %[thresh])

    "sub        %[p], %[p], %[stride], lsl #1  \n"  

    "vst1.u8    {q2}, [%[p]], %[stride]        \n"  
    "vst1.u8    {q3}, [%[p]]                   \n"  
    : [p] "+r"(p)
    : [stride] "r"(stride), [thresh] "r"(thresh)
    : "memory", QRegs
  );
}

static void SimpleHFilter16NEON(uint8_t* p, int stride, int thresh) {
  __asm__ volatile (
    "sub        r4, %[p], #2                   \n"  
    "lsl        r6, %[stride], #1              \n"  
    "add        r5, r4, %[stride]              \n"  

    LOAD8x4(d2, d3, d4, d5, [r4], [r5], r6)
    LOAD8x4(d6, d7, d8, d9, [r4], [r5], r6)
    "vswp       d3, d6                         \n"  
    "vswp       d5, d8                         \n"  
    "vswp       q2, q3                         \n"  

    DO_FILTER2(q1, q2, q3, q4, %[thresh])

    "sub        %[p], %[p], #1                 \n"  

    "vswp        d5, d6                        \n"
    STORE8x2(d4, d5, [%[p]], %[stride])
    STORE8x2(d6, d7, [%[p]], %[stride])

    : [p] "+r"(p)
    : [stride] "r"(stride), [thresh] "r"(thresh)
    : "memory", "r4", "r5", "r6", QRegs
  );
}

static void SimpleVFilter16iNEON(uint8_t* p, int stride, int thresh) {
  int k;
  for (k = 3; k > 0; --k) {
    p += 4 * stride;
    SimpleVFilter16NEON(p, stride, thresh);
  }
}

static void SimpleHFilter16iNEON(uint8_t* p, int stride, int thresh) {
  int k;
  for (k = 3; k > 0; --k) {
    p += 4;
    SimpleHFilter16NEON(p, stride, thresh);
  }
}


static void TransformOneNEON(const int16_t *in, uint8_t *dst) {
  const int kBPS = BPS;
  const int16_t constants[] = {20091, 17734, 0, 0};


  
  __asm__ volatile (
    "vld1.16         {q1, q2}, [%[in]]           \n"
    "vld1.16         {d0}, [%[constants]]        \n"

    "vswp            d3, d4                      \n"

    "vqdmulh.s16     q8, q2, d0[0]               \n"
    "vqdmulh.s16     q9, q2, d0[1]               \n"

    "vqadd.s16       d22, d2, d3                 \n"
    "vqsub.s16       d23, d2, d3                 \n"

    "vshr.s16        q8, q8, #1                  \n"

    "vqadd.s16       q8, q2, q8                  \n"

    "vqsub.s16       d20, d18, d17               \n"
    "vqadd.s16       d21, d19, d16               \n"

    "vqadd.s16       d2, d22, d21                \n"
    "vqadd.s16       d3, d23, d20                \n"
    "vqsub.s16       d4, d23, d20                \n"
    "vqsub.s16       d5, d22, d21                \n"

    "vzip.16         q1, q2                      \n"
    "vzip.16         q1, q2                      \n"

    "vswp            d3, d4                      \n"

    "vqdmulh.s16     q8, q2, d0[0]               \n"
    "vqdmulh.s16     q9, q2, d0[1]               \n"

    "vqadd.s16       d22, d2, d3                 \n"
    "vqsub.s16       d23, d2, d3                 \n"

    
    "vshr.s16        q8, q8, #1                  \n"
    "vqadd.s16       q8, q2, q8                  \n"

    "vqsub.s16       d20, d18, d17               \n"
    "vqadd.s16       d21, d19, d16               \n"

    "vqadd.s16       d2, d22, d21                \n"
    "vqadd.s16       d3, d23, d20                \n"
    "vqsub.s16       d4, d23, d20                \n"
    "vqsub.s16       d5, d22, d21                \n"

    "vld1.32         d6[0], [%[dst]], %[kBPS]    \n"
    "vld1.32         d6[1], [%[dst]], %[kBPS]    \n"
    "vld1.32         d7[0], [%[dst]], %[kBPS]    \n"
    "vld1.32         d7[1], [%[dst]], %[kBPS]    \n"

    "sub         %[dst], %[dst], %[kBPS], lsl #2 \n"

    
    "vrshr.s16       d2, d2, #3                  \n"
    "vrshr.s16       d3, d3, #3                  \n"
    "vrshr.s16       d4, d4, #3                  \n"
    "vrshr.s16       d5, d5, #3                  \n"

    "vzip.16         q1, q2                      \n"
    "vzip.16         q1, q2                      \n"

    
    "vmovl.u8        q8, d6                      \n"
    "vmovl.u8        q9, d7                      \n"

    "vqadd.s16       q1, q1, q8                  \n"
    "vqadd.s16       q2, q2, q9                  \n"

    "vqmovun.s16     d0, q1                      \n"
    "vqmovun.s16     d1, q2                      \n"

    "vst1.32         d0[0], [%[dst]], %[kBPS]    \n"
    "vst1.32         d0[1], [%[dst]], %[kBPS]    \n"
    "vst1.32         d1[0], [%[dst]], %[kBPS]    \n"
    "vst1.32         d1[1], [%[dst]]             \n"

    : [in] "+r"(in), [dst] "+r"(dst)  
    : [kBPS] "r"(kBPS), [constants] "r"(constants)  
    : "memory", "q0", "q1", "q2", "q8", "q9", "q10", "q11"  
  );
}

static void TransformTwoNEON(const int16_t* in, uint8_t* dst, int do_two) {
  TransformOneNEON(in, dst);
  if (do_two) {
    TransformOneNEON(in + 16, dst + 4);
  }
}

static void TransformWHT(const int16_t* in, int16_t* out) {
  const int kStep = 32;  
                         
  __asm__ volatile (
    
    
    "vld1.16         {q0, q1}, [%[in]]           \n"

    "vaddl.s16       q2, d0, d3                  \n" 
    "vaddl.s16       q3, d1, d2                  \n" 
    "vsubl.s16       q4, d1, d2                  \n" 
    "vsubl.s16       q5, d0, d3                  \n" 

    "vadd.s32        q0, q2, q3                  \n" 
    "vsub.s32        q2, q2, q3                  \n" 
    "vadd.s32        q1, q5, q4                  \n" 
    "vsub.s32        q3, q5, q4                  \n" 

    
    
    
    "vswp            d1, d4                      \n" 
    "vswp            d3, d6                      \n" 
    "vtrn.32         q0, q1                      \n"
    "vtrn.32         q2, q3                      \n"

    "vmov.s32        q4, #3                      \n" 
    "vadd.s32        q0, q0, q4                  \n" 
    "vadd.s32        q6, q0, q3                  \n" 
    "vadd.s32        q7, q1, q2                  \n" 
    "vsub.s32        q8, q1, q2                  \n" 
    "vsub.s32        q9, q0, q3                  \n" 

    "vadd.s32        q0, q6, q7                  \n"
    "vshrn.s32       d0, q0, #3                  \n" 
    "vadd.s32        q1, q9, q8                  \n"
    "vshrn.s32       d1, q1, #3                  \n" 
    "vsub.s32        q2, q6, q7                  \n"
    "vshrn.s32       d2, q2, #3                  \n" 
    "vsub.s32        q3, q9, q8                  \n"
    "vshrn.s32       d3, q3, #3                  \n" 

    
    "vst1.16         d0[0], [%[out]], %[kStep]   \n"
    "vst1.16         d1[0], [%[out]], %[kStep]   \n"
    "vst1.16         d2[0], [%[out]], %[kStep]   \n"
    "vst1.16         d3[0], [%[out]], %[kStep]   \n"
    "vst1.16         d0[1], [%[out]], %[kStep]   \n"
    "vst1.16         d1[1], [%[out]], %[kStep]   \n"
    "vst1.16         d2[1], [%[out]], %[kStep]   \n"
    "vst1.16         d3[1], [%[out]], %[kStep]   \n"
    "vst1.16         d0[2], [%[out]], %[kStep]   \n"
    "vst1.16         d1[2], [%[out]], %[kStep]   \n"
    "vst1.16         d2[2], [%[out]], %[kStep]   \n"
    "vst1.16         d3[2], [%[out]], %[kStep]   \n"
    "vst1.16         d0[3], [%[out]], %[kStep]   \n"
    "vst1.16         d1[3], [%[out]], %[kStep]   \n"
    "vst1.16         d2[3], [%[out]], %[kStep]   \n"
    "vst1.16         d3[3], [%[out]], %[kStep]   \n"

    : [out] "+r"(out)  
    : [in] "r"(in), [kStep] "r"(kStep)  
    : "memory", "q0", "q1", "q2", "q3", "q4",
      "q5", "q6", "q7", "q8", "q9"  
  );
}

#endif   


extern void VP8DspInitNEON(void);

void VP8DspInitNEON(void) {
#if defined(WEBP_USE_NEON)
  VP8Transform = TransformTwoNEON;
  VP8TransformWHT = TransformWHT;

  VP8SimpleVFilter16 = SimpleVFilter16NEON;
  VP8SimpleHFilter16 = SimpleHFilter16NEON;
  VP8SimpleVFilter16i = SimpleVFilter16iNEON;
  VP8SimpleHFilter16i = SimpleHFilter16iNEON;
#endif   
}

#if defined(__cplusplus) || defined(c_plusplus)
}    
#endif
