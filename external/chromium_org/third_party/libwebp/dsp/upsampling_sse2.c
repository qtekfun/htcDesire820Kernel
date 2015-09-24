// Copyright 2011 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#include "./dsp.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if defined(WEBP_USE_SSE2)

#include <assert.h>
#include <emmintrin.h>
#include <string.h>
#include "./yuv.h"

#ifdef FANCY_UPSAMPLING

// Then m can be written as

#define GET_M(ij, in, out) do {                                                \
  const __m128i tmp0 = _mm_avg_epu8(k, (in));            \
  const __m128i tmp1 = _mm_and_si128((ij), st);              \
  const __m128i tmp2 = _mm_xor_si128(k, (in));                     \
  const __m128i tmp3 = _mm_or_si128(tmp1, tmp2);  \
  const __m128i tmp4 = _mm_and_si128(tmp3, one);    \
  (out) = _mm_sub_epi8(tmp0, tmp4);     \
} while (0)

#define PACK_AND_STORE(a, b, da, db, out) do {                                 \
  const __m128i t_a = _mm_avg_epu8(a, da);   \
  const __m128i t_b = _mm_avg_epu8(b, db);   \
  const __m128i t_1 = _mm_unpacklo_epi8(t_a, t_b);                             \
  const __m128i t_2 = _mm_unpackhi_epi8(t_a, t_b);                             \
  _mm_store_si128(((__m128i*)(out)) + 0, t_1);                                 \
  _mm_store_si128(((__m128i*)(out)) + 1, t_2);                                 \
} while (0)

#define UPSAMPLE_32PIXELS(r1, r2, out) {                                       \
  const __m128i one = _mm_set1_epi8(1);                                        \
  const __m128i a = _mm_loadu_si128((__m128i*)&(r1)[0]);                       \
  const __m128i b = _mm_loadu_si128((__m128i*)&(r1)[1]);                       \
  const __m128i c = _mm_loadu_si128((__m128i*)&(r2)[0]);                       \
  const __m128i d = _mm_loadu_si128((__m128i*)&(r2)[1]);                       \
                                                                               \
  const __m128i s = _mm_avg_epu8(a, d);               \
  const __m128i t = _mm_avg_epu8(b, c);               \
  const __m128i st = _mm_xor_si128(s, t);                        \
                                                                               \
  const __m128i ad = _mm_xor_si128(a, d);                        \
  const __m128i bc = _mm_xor_si128(b, c);                        \
                                                                               \
  const __m128i t1 = _mm_or_si128(ad, bc);                  \
  const __m128i t2 = _mm_or_si128(t1, st);          \
  const __m128i t3 = _mm_and_si128(t2, one);    \
  const __m128i t4 = _mm_avg_epu8(s, t);                                       \
  const __m128i k = _mm_sub_epi8(t4, t3);         \
  __m128i diag1, diag2;                                                        \
                                                                               \
  GET_M(bc, t, diag1);                      \
  GET_M(ad, s, diag2);                      \
                                                                               \
                                                \
  PACK_AND_STORE(a, b, diag1, diag2, &(out)[0 * 32]);                          \
  PACK_AND_STORE(c, d, diag2, diag1, &(out)[2 * 32]);                          \
}

static void Upsample32Pixels(const uint8_t r1[], const uint8_t r2[],
                             uint8_t* const out) {
  UPSAMPLE_32PIXELS(r1, r2, out);
}

#define UPSAMPLE_LAST_BLOCK(tb, bb, num_pixels, out) {                         \
  uint8_t r1[17], r2[17];                                                      \
  memcpy(r1, (tb), (num_pixels));                                              \
  memcpy(r2, (bb), (num_pixels));                                              \
                                                      \
  memset(r1 + (num_pixels), r1[(num_pixels) - 1], 17 - (num_pixels));          \
  memset(r2 + (num_pixels), r2[(num_pixels) - 1], 17 - (num_pixels));          \
       \
  Upsample32Pixels(r1, r2, out);                                               \
}

#define CONVERT2RGB(FUNC, XSTEP, top_y, bottom_y, uv,                          \
                    top_dst, bottom_dst, cur_x, num_pixels) {                  \
  int n;                                                                       \
  if (top_y) {                                                                 \
    for (n = 0; n < (num_pixels); ++n) {                                       \
      FUNC(top_y[(cur_x) + n], (uv)[n], (uv)[32 + n],                          \
           top_dst + ((cur_x) + n) * XSTEP);                                   \
    }                                                                          \
  }                                                                            \
  if (bottom_y) {                                                              \
    for (n = 0; n < (num_pixels); ++n) {                                       \
      FUNC(bottom_y[(cur_x) + n], (uv)[64 + n], (uv)[64 + 32 + n],             \
           bottom_dst + ((cur_x) + n) * XSTEP);                                \
    }                                                                          \
  }                                                                            \
}

#define SSE2_UPSAMPLE_FUNC(FUNC_NAME, FUNC, XSTEP)                             \
static void FUNC_NAME(const uint8_t* top_y, const uint8_t* bottom_y,           \
                      const uint8_t* top_u, const uint8_t* top_v,              \
                      const uint8_t* cur_u, const uint8_t* cur_v,              \
                      uint8_t* top_dst, uint8_t* bottom_dst, int len) {        \
  int block;                                                                   \
                     \
  uint8_t uv_buf[4 * 32 + 15];                                                 \
  uint8_t* const r_uv = (uint8_t*)((uintptr_t)(uv_buf + 15) & ~15);            \
  const int uv_len = (len + 1) >> 1;                                           \
                               \
  const int num_blocks = (uv_len - 1) >> 4;                                    \
  const int leftover = uv_len - num_blocks * 16;                               \
  const int last_pos = 1 + 32 * num_blocks;                                    \
                                                                               \
  const int u_diag = ((top_u[0] + cur_u[0]) >> 1) + 1;                         \
  const int v_diag = ((top_v[0] + cur_v[0]) >> 1) + 1;                         \
                                                                               \
  assert(len > 0);                                                             \
                                     \
  if (top_y) {                                                                 \
    const int u0 = (top_u[0] + u_diag) >> 1;                                   \
    const int v0 = (top_v[0] + v_diag) >> 1;                                   \
    FUNC(top_y[0], u0, v0, top_dst);                                           \
  }                                                                            \
  if (bottom_y) {                                                              \
    const int u0 = (cur_u[0] + u_diag) >> 1;                                   \
    const int v0 = (cur_v[0] + v_diag) >> 1;                                   \
    FUNC(bottom_y[0], u0, v0, bottom_dst);                                     \
  }                                                                            \
                                                                               \
  for (block = 0; block < num_blocks; ++block) {                               \
    UPSAMPLE_32PIXELS(top_u, cur_u, r_uv + 0 * 32);                            \
    UPSAMPLE_32PIXELS(top_v, cur_v, r_uv + 1 * 32);                            \
    CONVERT2RGB(FUNC, XSTEP, top_y, bottom_y, r_uv, top_dst, bottom_dst,       \
                32 * block + 1, 32)                                            \
    top_u += 16;                                                               \
    cur_u += 16;                                                               \
    top_v += 16;                                                               \
    cur_v += 16;                                                               \
  }                                                                            \
                                                                               \
  UPSAMPLE_LAST_BLOCK(top_u, cur_u, leftover, r_uv + 0 * 32);                  \
  UPSAMPLE_LAST_BLOCK(top_v, cur_v, leftover, r_uv + 1 * 32);                  \
  CONVERT2RGB(FUNC, XSTEP, top_y, bottom_y, r_uv, top_dst, bottom_dst,         \
              last_pos, len - last_pos);                                       \
}

SSE2_UPSAMPLE_FUNC(UpsampleRgbLinePairSSE2,  VP8YuvToRgb,  3)
SSE2_UPSAMPLE_FUNC(UpsampleBgrLinePairSSE2,  VP8YuvToBgr,  3)
SSE2_UPSAMPLE_FUNC(UpsampleRgbaLinePairSSE2, VP8YuvToRgba, 4)
SSE2_UPSAMPLE_FUNC(UpsampleBgraLinePairSSE2, VP8YuvToBgra, 4)

#undef GET_M
#undef PACK_AND_STORE
#undef UPSAMPLE_32PIXELS
#undef UPSAMPLE_LAST_BLOCK
#undef CONVERT2RGB
#undef SSE2_UPSAMPLE_FUNC

#endif  

#endif   


extern WebPUpsampleLinePairFunc WebPUpsamplers[];

void WebPInitUpsamplersSSE2(void) {
#if defined(WEBP_USE_SSE2)
  WebPUpsamplers[MODE_RGB]  = UpsampleRgbLinePairSSE2;
  WebPUpsamplers[MODE_RGBA] = UpsampleRgbaLinePairSSE2;
  WebPUpsamplers[MODE_BGR]  = UpsampleBgrLinePairSSE2;
  WebPUpsamplers[MODE_BGRA] = UpsampleBgraLinePairSSE2;
#endif   
}

void WebPInitPremultiplySSE2(void) {
#if defined(WEBP_USE_SSE2)
  WebPUpsamplers[MODE_rgbA] = UpsampleRgbaLinePairSSE2;
  WebPUpsamplers[MODE_bgrA] = UpsampleBgraLinePairSSE2;
#endif   
}

#if defined(__cplusplus) || defined(c_plusplus)
}    
#endif


