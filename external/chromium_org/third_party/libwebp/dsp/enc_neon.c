// Copyright 2012 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#include "./dsp.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if defined(WEBP_USE_NEON)

#include "../enc/vp8enci.h"


static void ITransformOne(const uint8_t* ref,
                          const int16_t* in, uint8_t* dst) {
  const int kBPS = BPS;
  const int16_t kC1C2[] = { 20091, 17734, 0, 0 };  

  __asm__ volatile (
    "vld1.16         {q1, q2}, [%[in]]           \n"
    "vld1.16         {d0}, [%[kC1C2]]            \n"

    
    
    
    
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

    "vld1.32         d6[0], [%[ref]], %[kBPS]    \n"
    "vld1.32         d6[1], [%[ref]], %[kBPS]    \n"
    "vld1.32         d7[0], [%[ref]], %[kBPS]    \n"
    "vld1.32         d7[1], [%[ref]], %[kBPS]    \n"

    "sub         %[ref], %[ref], %[kBPS], lsl #2 \n"

    
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
    : [kBPS] "r"(kBPS), [kC1C2] "r"(kC1C2), [ref] "r"(ref)  
    : "memory", "q0", "q1", "q2", "q8", "q9", "q10", "q11"  
  );
}

static void ITransform(const uint8_t* ref,
                       const int16_t* in, uint8_t* dst, int do_two) {
  ITransformOne(ref, in, dst);
  if (do_two) {
    ITransformOne(ref + 4, in + 16, dst + 4);
  }
}

static void ITransformWHT(const int16_t* in, int16_t* out) {
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

    
    "vst1.16         d0[0], [%[out]], %[kStep]      \n"
    "vst1.16         d1[0], [%[out]], %[kStep]      \n"
    "vst1.16         d2[0], [%[out]], %[kStep]      \n"
    "vst1.16         d3[0], [%[out]], %[kStep]      \n"
    "vst1.16         d0[1], [%[out]], %[kStep]      \n"
    "vst1.16         d1[1], [%[out]], %[kStep]      \n"
    "vst1.16         d2[1], [%[out]], %[kStep]      \n"
    "vst1.16         d3[1], [%[out]], %[kStep]      \n"
    "vst1.16         d0[2], [%[out]], %[kStep]      \n"
    "vst1.16         d1[2], [%[out]], %[kStep]      \n"
    "vst1.16         d2[2], [%[out]], %[kStep]      \n"
    "vst1.16         d3[2], [%[out]], %[kStep]      \n"
    "vst1.16         d0[3], [%[out]], %[kStep]      \n"
    "vst1.16         d1[3], [%[out]], %[kStep]      \n"
    "vst1.16         d2[3], [%[out]], %[kStep]      \n"
    "vst1.16         d3[3], [%[out]], %[kStep]      \n"

    : [out] "+r"(out)  
    : [in] "r"(in), [kStep] "r"(kStep)  
    : "memory", "q0", "q1", "q2", "q3", "q4",
      "q5", "q6", "q7", "q8", "q9" 
  );
}


static const int16_t kCoeff16[] = {
  5352,  5352,  5352, 5352, 2217,  2217,  2217, 2217
};
static const int32_t kCoeff32[] = {
   1812,  1812,  1812,  1812,
    937,   937,   937,   937,
  12000, 12000, 12000, 12000,
  51000, 51000, 51000, 51000
};

static void FTransform(const uint8_t* src, const uint8_t* ref,
                       int16_t* out) {
  const int kBPS = BPS;
  const uint8_t* src_ptr = src;
  const uint8_t* ref_ptr = ref;
  const int16_t* coeff16 = kCoeff16;
  const int32_t* coeff32 = kCoeff32;

  __asm__ volatile (
    
    "vld1.8 {d8},  [%[src_ptr]], %[kBPS]      \n"
    "vld1.8 {d10}, [%[src_ptr]], %[kBPS]      \n"
    "vld1.8 {d9},  [%[src_ptr]], %[kBPS]      \n"
    "vld1.8 {d11}, [%[src_ptr]]               \n"

    
    "vld1.8 {d12}, [%[ref_ptr]], %[kBPS]      \n"
    "vld1.8 {d14}, [%[ref_ptr]], %[kBPS]      \n"
    "vld1.8 {d13}, [%[ref_ptr]], %[kBPS]      \n"
    "vld1.8 {d15}, [%[ref_ptr]]               \n"

    
    "vtrn.32     q4, q5                       \n"
    "vtrn.32     q6, q7                       \n"

    
    "vsubl.u8    q0, d8, d12                  \n"
    "vsubl.u8    q1, d9, d13                  \n"

    
    "vld1.16     {q8}, [%[coeff16]]           \n"

    
    "vld1.32     {q9, q10}, [%[coeff32]]!     \n"

    
    "vld1.32     {q11,q12}, [%[coeff32]]      \n"

    
    
    "vtrn.32         d0, d2                   \n"
    "vtrn.32         d1, d3                   \n"
    "vtrn.16         d0, d1                   \n"
    "vtrn.16         d2, d3                   \n"

    "vadd.s16        d4, d0, d3               \n" 
    "vadd.s16        d5, d1, d2               \n" 
    "vsub.s16        d6, d1, d2               \n" 
    "vsub.s16        d7, d0, d3               \n" 

    "vadd.s16        d0, d4, d5               \n" 
    "vshl.s16        d0, d0, #3               \n" 
    "vsub.s16        d2, d4, d5               \n" 
    "vshl.s16        d2, d2, #3               \n" 

    "vmlal.s16       q9, d7, d16              \n" 
    "vmlal.s16       q10, d7, d17             \n" 
    "vmlal.s16       q9, d6, d17              \n" 
    "vmlsl.s16       q10, d6, d16             \n" 

    
    
    "vshrn.s32       d1, q9, #9               \n"
    "vshrn.s32       d3, q10, #9              \n"

    
    
    "vtrn.32         d0, d2                   \n"
    "vtrn.32         d1, d3                   \n"
    "vtrn.16         d0, d1                   \n"
    "vtrn.16         d2, d3                   \n"

    "vmov.s16        d26, #7                  \n"

    "vadd.s16        d4, d0, d3               \n" 
    "vadd.s16        d5, d1, d2               \n" 
    "vsub.s16        d6, d1, d2               \n" 
    "vadd.s16        d4, d4, d26              \n" 
    "vsub.s16        d7, d0, d3               \n" 

    "vadd.s16        d0, d4, d5               \n" 
    "vsub.s16        d2, d4, d5               \n" 

    "vmlal.s16       q11, d7, d16             \n" 
    "vmlal.s16       q12, d7, d17             \n" 

    "vceq.s16        d4, d7, #0               \n"

    "vshr.s16        d0, d0, #4               \n"
    "vshr.s16        d2, d2, #4               \n"

    "vmlal.s16       q11, d6, d17             \n" 
    "vmlsl.s16       q12, d6, d16             \n" 

    "vmvn            d4, d4                   \n" 
    
    "vshrn.s32       d1, q11, #16             \n"
    
    "vsub.s16        d1, d1, d4               \n"
    
    "vshrn.s32       d3, q12, #16             \n"

    
    "vst1.16         {q0, q1}, [%[out]]   \n"
    : [src_ptr] "+r"(src_ptr), [ref_ptr] "+r"(ref_ptr),
      [coeff32] "+r"(coeff32)          
    : [kBPS] "r"(kBPS), [coeff16] "r"(coeff16),
      [out] "r"(out)                   
    : "memory", "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q9",
      "q10", "q11", "q12", "q13"       
  );
}

static void FTransformWHT(const int16_t* in, int16_t* out) {
  const int kStep = 32;
  __asm__ volatile (
    
    
    "vld1.16         d0[0], [%[in]], %[kStep]   \n"
    "vld1.16         d1[0], [%[in]], %[kStep]   \n"
    "vld1.16         d2[0], [%[in]], %[kStep]   \n"
    "vld1.16         d3[0], [%[in]], %[kStep]   \n"
    "vld1.16         d0[1], [%[in]], %[kStep]   \n"
    "vld1.16         d1[1], [%[in]], %[kStep]   \n"
    "vld1.16         d2[1], [%[in]], %[kStep]   \n"
    "vld1.16         d3[1], [%[in]], %[kStep]   \n"
    "vld1.16         d0[2], [%[in]], %[kStep]   \n"
    "vld1.16         d1[2], [%[in]], %[kStep]   \n"
    "vld1.16         d2[2], [%[in]], %[kStep]   \n"
    "vld1.16         d3[2], [%[in]], %[kStep]   \n"
    "vld1.16         d0[3], [%[in]], %[kStep]   \n"
    "vld1.16         d1[3], [%[in]], %[kStep]   \n"
    "vld1.16         d2[3], [%[in]], %[kStep]   \n"
    "vld1.16         d3[3], [%[in]], %[kStep]   \n"

    "vaddl.s16       q2, d0, d2                 \n" 
    "vaddl.s16       q3, d1, d3                 \n" 
    "vsubl.s16       q4, d1, d3                 \n" 
    "vsubl.s16       q5, d0, d2                 \n" 

    "vqadd.s32       q6, q2, q3                 \n" 
    "vqadd.s32       q7, q5, q4                 \n" 
    "vqsub.s32       q8, q5, q4                 \n" 
    "vqsub.s32       q9, q2, q3                 \n" 

    
    
    
    "vswp            d13, d16                   \n" 
    "vswp            d15, d18                   \n" 
    "vtrn.32         q6, q7                     \n"
    "vtrn.32         q8, q9                     \n"

    "vqadd.s32       q0, q6, q8                 \n" 
    "vqadd.s32       q1, q7, q9                 \n" 
    "vqsub.s32       q2, q7, q9                 \n" 
    "vqsub.s32       q3, q6, q8                 \n" 

    "vqadd.s32       q4, q0, q1                 \n" 
    "vqadd.s32       q5, q3, q2                 \n" 
    "vqsub.s32       q6, q3, q2                 \n" 
    "vqsub.s32       q7, q0, q1                 \n" 

    "vshrn.s32       d18, q4, #1                \n" 
    "vshrn.s32       d19, q5, #1                \n" 
    "vshrn.s32       d20, q6, #1                \n" 
    "vshrn.s32       d21, q7, #1                \n" 

    "vst1.16         {q9, q10}, [%[out]]        \n"

    : [in] "+r"(in)
    : [kStep] "r"(kStep), [out] "r"(out)
    : "memory", "q0", "q1", "q2", "q3", "q4", "q5",
      "q6", "q7", "q8", "q9", "q10"       
  ) ;
}


static int Disto4x4(const uint8_t* const a, const uint8_t* const b,
                    const uint16_t* const w) {
  const int kBPS = BPS;
  const uint8_t* A = a;
  const uint8_t* B = b;
  const uint16_t* W = w;
  int sum;
  __asm__ volatile (
    "vld1.32         d0[0], [%[a]], %[kBPS]   \n"
    "vld1.32         d0[1], [%[a]], %[kBPS]   \n"
    "vld1.32         d2[0], [%[a]], %[kBPS]   \n"
    "vld1.32         d2[1], [%[a]]            \n"

    "vld1.32         d1[0], [%[b]], %[kBPS]   \n"
    "vld1.32         d1[1], [%[b]], %[kBPS]   \n"
    "vld1.32         d3[0], [%[b]], %[kBPS]   \n"
    "vld1.32         d3[1], [%[b]]            \n"

    
    
    
    
    
    "vtrn.16         q0, q1                   \n"

    
    "vaddl.u8        q2, d0, d2               \n"
    "vaddl.u8        q10, d1, d3              \n"
    
    "vsubl.u8        q3, d0, d2               \n"
    "vsubl.u8        q11, d1, d3              \n"

    
    "vpaddl.s16      q0, q2                   \n"
    "vpaddl.s16      q8, q10                  \n"

    
    "vpaddl.s16      q1, q3                   \n"
    "vpaddl.s16      q9, q11                  \n"

    
    
    
    "vtrn.16         q2, q3                   \n"
    "vtrn.16         q10, q11                 \n"

    
    "vsubl.s16       q12, d4, d6              \n"
    "vsubl.s16       q13, d5, d7              \n"
    "vsubl.s16       q14, d20, d22            \n"
    "vsubl.s16       q15, d21, d23            \n"

    
    
    
    "vtrn.32         q12, q13                 \n"
    "vtrn.32         q14, q15                 \n"

    
    "vswp            d1, d26                  \n" 
    "vswp            d3, d24                  \n" 
    "vtrn.32         q0, q1                   \n"
    "vtrn.32         q13, q12                 \n"

    
    "vswp            d17, d30                 \n" 
    "vswp            d19, d28                 \n" 
    "vtrn.32         q8, q9                   \n"
    "vtrn.32         q15, q14                 \n"

    
    
    
    
    

    
    
    "vswp            d3, d27                  \n"
    "vswp            d19, d31                 \n"

    
    "vadd.s32        q2, q0, q1               \n"
    "vadd.s32        q3, q8, q9               \n"

    
    "vadd.s32        q10, q13, q12            \n"
    "vadd.s32        q11, q15, q14            \n"

    
    "vsub.s32        q13, q13, q12            \n"
    "vsub.s32        q15, q15, q14            \n"

    
    "vsub.s32        q0, q0, q1               \n"
    "vsub.s32        q8, q8, q9               \n"

    
    "vadd.s32        q1, q2, q10              \n"
    "vadd.s32        q9, q3, q11              \n"

    
    "vadd.s32        q12, q0, q13             \n"
    "vadd.s32        q14, q8, q15             \n"

    
    "vsub.s32        q0, q0, q13              \n"
    "vsub.s32        q8, q8, q15              \n"

    
    "vsub.s32        q2, q2, q10              \n"
    "vsub.s32        q3, q3, q11              \n"

    "vld1.64         {q10, q11}, [%[w]]       \n"

    
    "vabs.s32        q1, q1                   \n"
    "vabs.s32        q9, q9                   \n"
    
    "vabs.s32        q12, q12                 \n"
    "vabs.s32        q14, q14                 \n"
    
    "vabs.s32        q0, q0                   \n"
    "vabs.s32        q8, q8                   \n"
    
    "vabs.s32        q2, q2                   \n"
    "vabs.s32        q3, q3                   \n"

    
    "vmovl.u16       q13, d20                 \n"
    "vmovl.u16       q15, d21                 \n"

    
    "vmul.u32        q1, q1, q13              \n"
    "vmul.u32        q9, q9, q13              \n"

    
    "vmla.u32        q1, q12, q15             \n"
    "vmla.u32        q9, q14, q15             \n"

    
    "vmovl.u16       q13, d22                 \n"
    "vmovl.u16       q15, d23                 \n"

    
    "vmla.u32        q1, q0, q13              \n"
    "vmla.u32        q9, q8, q13              \n"

    
    "vmla.u32        q1, q2, q15              \n"
    "vmla.u32        q9, q3, q15              \n"

    
    "vpaddl.u32      q1, q1                   \n"
    "vpaddl.u32      q9, q9                   \n"
    "vadd.u64        d2, d3                   \n"
    "vadd.u64        d18, d19                 \n"

    
    
    
    
    
    
    

    
    "vsub.u32        d0, d2, d18              \n"
    
    "vabs.s32        d0, d0                   \n"
    
    "vshr.u32        d0, #5                   \n"

    
    
    "vmov.32         %[sum], d0[0]            \n"

    : [sum] "=r"(sum), [a] "+r"(A), [b] "+r"(B), [w] "+r"(W)
    : [kBPS] "r"(kBPS)
    : "memory", "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q9",
      "q10", "q11", "q12", "q13", "q14", "q15"  
  ) ;

  return sum;
}

static int Disto16x16(const uint8_t* const a, const uint8_t* const b,
                      const uint16_t* const w) {
  int D = 0;
  int x, y;
  for (y = 0; y < 16 * BPS; y += 4 * BPS) {
    for (x = 0; x < 16; x += 4) {
      D += Disto4x4(a + x + y, b + x + y, w);
    }
  }
  return D;
}

#endif   


extern void VP8EncDspInitNEON(void);

void VP8EncDspInitNEON(void) {
#if defined(WEBP_USE_NEON)
  VP8ITransform = ITransform;
  VP8FTransform = FTransform;

  VP8ITransformWHT = ITransformWHT;
  VP8FTransformWHT = FTransformWHT;

  VP8TDisto4x4 = Disto4x4;
  VP8TDisto16x16 = Disto16x16;
#endif   
}

#if defined(__cplusplus) || defined(c_plusplus)
}    
#endif
