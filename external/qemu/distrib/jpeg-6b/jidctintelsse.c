/*
 * Copyright (C) 2010-2011 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"
#include "jdct.h"		

#ifdef ANDROID_INTELSSE2_IDCT
#include <emmintrin.h>

#if DCTSIZE != 8
  Sorry, this code only copes with 8x8 DCTs. 
#endif

#define BITS_INV_ACC 4
#define SHIFT_INV_ROW 12
#define SHIFT_INV_COL 5
const short RND_INV_ROW = 2048;
const short RND_INV_COL = 16;
const short RND_INV_CORR = 15;

static const short __attribute__ ((aligned(16))) M128_one_corr[8] = {1,1,1,1,1,1,1,1};
static const short __attribute__ ((aligned(16))) M128_round_inv_row[8] = {2048,0,2048,0,2048,0,2048,0};
static const short __attribute__ ((aligned(16))) M128_round_inv_col[8] = {16,16,16,16,16,16,16,16};
static const short __attribute__ ((aligned(16))) M128_round_inv_corr[8] = {15,15,15,15,15,15,15,15};

static const short __attribute__ ((aligned(16))) M128_tg_1_16[8] = {13036, 13036, 13036, 13036, 13036, 13036, 13036, 13036};
static const short __attribute__ ((aligned(16))) M128_tg_2_16[8] = {27146, 27146, 27146, 27146, 27146, 27146, 27146, 27146};
static const short __attribute__ ((aligned(16))) M128_tg_3_16[8] = {-21746, -21746, -21746, -21746, -21746, -21746, -21746, -21746};
static const short __attribute__ ((aligned(16))) M128_cos_4_16[8] = {-19195, -19195, -19195, -19195, -19195, -19195, -19195, -19195};

static const short __attribute__ ((aligned(16))) jpeg_adjust[8] = {128, 128, 128, 128, 128, 128, 128, 128};

static const short __attribute__ ((aligned(16))) M128_tab_i_04[32] = {
16384, 21407, 16384, 8867,
16384, -8867, 16384, -21407,
16384, 8867, -16384, -21407,
-16384, 21407, 16384, -8867,
22725, 19266, 19266, -4520,
12873, -22725, 4520, -12873,
12873, 4520, -22725, -12873,
4520, 19266, 19266, -22725
};

static const short __attribute__ ((aligned(16))) M128_tab_i_17[32] = {
22725, 29692, 22725, 12299,
22725, -12299, 22725, -29692,
22725, 12299, -22725, -29692,
-22725, 29692, 22725, -12299,
31521, 26722, 26722, -6270,
17855, -31521, 6270, -17855,
17855, 6270, -31521, -17855,
6270, 26722, 26722, -31521
};

static const short __attribute__ ((aligned(16))) M128_tab_i_26[32] = {
21407, 27969, 21407, 11585,
21407, -11585, 21407, -27969,
21407, 11585, -21407, -27969,
-21407, 27969, 21407, -11585,
29692, 25172, 25172, -5906,
16819, -29692, 5906, -16819,
16819, 5906, -29692, -16819,
5906, 25172, 25172, -29692
};

static const short __attribute__ ((aligned(16))) M128_tab_i_35[32] = {
19266, 25172, 19266, 10426,
19266, -10426, 19266, -25172,
19266, 10426, -19266, -25172,
-19266, 25172, 19266, -10426,
26722, 22654, 22654, -5315,
15137, -26722, 5315, -15137,
15137, 5315, -26722, -15137,
5315, 22654, 22654, -26722
};



GLOBAL(void)
jpeg_idct_intelsse (j_decompress_ptr cinfo, jpeg_component_info * compptr,
		 JCOEFPTR coef_block,
		 JSAMPARRAY output_buf, JDIMENSION output_col)
{
  __m128i row0, tmp1, tmp2, tmp3, row2, tmp5, tmp6, tmp7;
  int ctr;
  JSAMPROW  outptrTemp;
  JSAMPLE *range_limit = IDCT_range_limit(cinfo);
  short __attribute__((aligned(16))) quantptrSSE[DCTSIZE2];
  short __attribute__((aligned(16))) workspaceSSE[DCTSIZE2];
  short __attribute__((aligned(16))) coef_blockSSE[DCTSIZE2];
  __m128i x0, x1, x2, x3, x4, x5, x6, x7;
  __m128i* tg3, *tg1, *tg2, *cos4;
  __m128i tm765, tp765, tm465, tp465, tp03, tm03, tp12, tm12, tp65, tm65;
  __m128i t0, t1, t2, t3, t4, t5, t6, t7;
  __m128i temp, temp2;
  short * wsptr;
  unsigned char * outptr;

#define iDCT_8_2ROWs(table1, table2)   \
    row0 = _mm_shufflelo_epi16(row0, 0xD8);     \
    row2 = _mm_shufflelo_epi16(row2, 0xD8);   \
    tmp1 = _mm_shuffle_epi32(row0, 0);          \
    tmp5 = _mm_shuffle_epi32(row2, 0);        \
                                                                                  \
    tmp3 = _mm_shuffle_epi32(row0, 0x55);       \
    tmp7 = _mm_shuffle_epi32(row2, 0x55);     \
    row0 = _mm_shufflehi_epi16(row0, 0xD8);     \
    row2 = _mm_shufflehi_epi16(row2, 0xD8);   \
						\
    tmp1 = _mm_madd_epi16(tmp1, * ( __m128i*)table1);         \
    tmp5 = _mm_madd_epi16(tmp5, * ( __m128i*)table2);       \
						\
    tmp2 =  _mm_shuffle_epi32(row0, 0xAA);      \
    tmp6 = _mm_shuffle_epi32(row2, 0xAA);     \
    row0 = _mm_shuffle_epi32(row0, 0xFF);       \
    row2 = _mm_shuffle_epi32(row2, 0xFF);     \
\
    tmp3 = _mm_madd_epi16(tmp3, * ( __m128i*)(table1+16));   \
    tmp7 = _mm_madd_epi16(tmp7, * ( __m128i*)(table2+16) ); \
    row0 = _mm_madd_epi16(row0, * ( __m128i*)(table1+24));   \
    row2 = _mm_madd_epi16(row2, * ( __m128i*)(table2+24) ); \
    tmp2 = _mm_madd_epi16(tmp2, * ( __m128i*)(table1+8) );   \
    tmp6 = _mm_madd_epi16(tmp6, * ( __m128i*)(table2+8) );  \
                                                             \
    tmp1 = _mm_add_epi32(tmp1, * ( __m128i*)M128_round_inv_row);       \
    tmp5 = _mm_add_epi32(tmp5, * ( __m128i*)M128_round_inv_row);      \
    row0 = _mm_add_epi32(row0, tmp3);      \
    row2 = _mm_add_epi32(row2, tmp7);                       \
    tmp1 = _mm_add_epi32(tmp1, tmp2);      \
    tmp5 = _mm_add_epi32(tmp5, tmp6);                       \
                                                             \
    tmp2 = tmp1;  \
    tmp6 = tmp5;  \
    tmp2 = _mm_sub_epi32(tmp2, row0);    \
    tmp6 = _mm_sub_epi32(tmp6, row2);  \
    row0 = _mm_add_epi32(row0, tmp1);    \
    row2 = _mm_add_epi32(row2, tmp5);  \
    tmp2 = _mm_srai_epi32(tmp2, SHIFT_INV_ROW);             \
    tmp6 = _mm_srai_epi32(tmp6, SHIFT_INV_ROW);  \
    row0 = _mm_srai_epi32(row0, SHIFT_INV_ROW);             \
    row2 = _mm_srai_epi32(row2, SHIFT_INV_ROW);  \
    tmp2 = _mm_shuffle_epi32(tmp2, 0x1B);    \
    tmp6 = _mm_shuffle_epi32(tmp6, 0x1B);  \
    row0 = _mm_packs_epi32(row0, tmp2);   \
    row2 = _mm_packs_epi32(row2, tmp6);  


#define iDCT_8_COL()  \
    x3 = _mm_load_si128(( __m128i*)(wsptr+24));\
    x1 = _mm_load_si128(( __m128i*)(wsptr+8));\
    x5 = row0;\
    x7 = row2;\
\
    tg3 = ( __m128i*)(M128_tg_3_16);\
    tg1 = ( __m128i*)(M128_tg_1_16);\
    tg2 = ( __m128i*)(M128_tg_2_16);\
    cos4 =(__m128i*)(M128_cos_4_16);\
\
    temp = _mm_mulhi_epi16(x5, *tg3);   \
    temp2 = _mm_mulhi_epi16(x3, *tg3);\
    temp = _mm_adds_epi16(temp, x5);  \
    temp2 = _mm_adds_epi16(temp2, x3);\
    tm765 = _mm_adds_epi16(temp, x3);\
    tm465 = _mm_subs_epi16(x5, temp2);\
\
    temp = _mm_mulhi_epi16(x7, *tg1);   \
    temp2 = _mm_mulhi_epi16(x1, *tg1);\
    tp765 = _mm_adds_epi16(temp, x1);\
    tp465 = _mm_subs_epi16(temp2, x7);   \
\
    t7 = _mm_adds_epi16(tp765, tm765);\
    t7 = _mm_adds_epi16(t7, *( __m128i*)M128_one_corr);\
    tp65 = _mm_subs_epi16(tp765, tm765);\
    t4 =  _mm_adds_epi16(tp465, tm465);\
    tm65 = _mm_subs_epi16(tp465, tm465);\
    tm65 = _mm_adds_epi16(tm65, *( __m128i*)M128_one_corr);\
\
    x0 = _mm_load_si128(( __m128i*)(wsptr));\
    x4 = _mm_load_si128(( __m128i*)(wsptr+32));\
    x2 = _mm_load_si128(( __m128i*)(wsptr+16));\
    x6 = _mm_load_si128(( __m128i*)(wsptr+48));\
\
     \
    temp = _mm_adds_epi16(tp65, tm65);\
    temp2 = _mm_subs_epi16(tp65, tm65);\
    t6 = _mm_mulhi_epi16(temp, *cos4);\
    t5 = _mm_mulhi_epi16(temp2, *cos4);\
    t6 = _mm_adds_epi16(t6, temp);\
    t6 = _mm_or_si128(t6, *( __m128i*)M128_one_corr);\
    t5 = _mm_adds_epi16(t5, temp2);\
    t5 = _mm_or_si128(t5, *( __m128i*)M128_one_corr);\
\
    tp03 = _mm_adds_epi16(x0, x4);\
    tp12 = _mm_subs_epi16(x0, x4);\
\
    temp = _mm_mulhi_epi16(x6, *tg2);\
    temp2 = _mm_mulhi_epi16(x2, *tg2);\
    tm03 = _mm_adds_epi16(temp, x2);\
    tm12 = _mm_subs_epi16(temp2, x6);\
\
    t0 = _mm_adds_epi16(tp03, tm03);\
    t0 = _mm_adds_epi16(t0, *( __m128i*)M128_round_inv_col);\
    t3 = _mm_subs_epi16(tp03, tm03);\
    t3 = _mm_adds_epi16(t3, *( __m128i*)M128_round_inv_corr);\
    t1 = _mm_adds_epi16(tp12, tm12);\
    t1 = _mm_adds_epi16(t1, *( __m128i*)M128_round_inv_col);\
    t2 = _mm_subs_epi16(tp12, tm12);\
    t2 = _mm_adds_epi16(t2, *( __m128i*)M128_round_inv_corr);\
\
    temp = _mm_adds_epi16(t0, t7);    \
    temp2 = _mm_adds_epi16(t1, t6);   \
    temp = _mm_srai_epi16(temp, SHIFT_INV_COL);\
    temp2 = _mm_srai_epi16(temp2, SHIFT_INV_COL);\
    temp = _mm_adds_epi16(temp, *( __m128i*)jpeg_adjust);  \
    temp2 = _mm_adds_epi16(temp2, *( __m128i*)jpeg_adjust);\
\
    temp = _mm_packus_epi16(temp, temp2);\
    _mm_store_si128(( __m128i*)(outptr), temp);   \
\
    temp = _mm_adds_epi16(t2, t5);\
    temp2 = _mm_adds_epi16(t3, t4);\
    temp = _mm_srai_epi16(temp, SHIFT_INV_COL);\
    temp2 = _mm_srai_epi16(temp2, SHIFT_INV_COL);\
    temp = _mm_adds_epi16(temp, *( __m128i*)jpeg_adjust);\
    temp2 = _mm_adds_epi16(temp2, *( __m128i*)jpeg_adjust);\
\
    temp = _mm_packus_epi16(temp, temp2);\
    _mm_store_si128(( __m128i*)(outptr+16), temp);   \
\
    temp = _mm_subs_epi16(t3, t4);\
    temp2 = _mm_subs_epi16(t2, t5);\
    temp = _mm_srai_epi16(temp, SHIFT_INV_COL);\
    temp2 = _mm_srai_epi16(temp2, SHIFT_INV_COL);\
    temp = _mm_adds_epi16(temp, *( __m128i*)jpeg_adjust);\
    temp2 = _mm_adds_epi16(temp2, *( __m128i*)jpeg_adjust);\
\
    temp = _mm_packus_epi16(temp, temp2);\
    _mm_store_si128(( __m128i*)(outptr+32), temp);   \
\
    temp = _mm_subs_epi16(t1, t6);\
    temp2 = _mm_subs_epi16(t0, t7);\
    temp = _mm_srai_epi16(temp, SHIFT_INV_COL);\
    temp2 = _mm_srai_epi16(temp2, SHIFT_INV_COL);\
    temp = _mm_adds_epi16(temp, *( __m128i*)jpeg_adjust);\
    temp2 = _mm_adds_epi16(temp2, *( __m128i*)jpeg_adjust);\
\
    temp = _mm_packus_epi16(temp, temp2);\
    _mm_store_si128(( __m128i*)(outptr+48), temp);  


  
  memcpy((char*)quantptrSSE, (char*)compptr->dct_table, sizeof(quantptrSSE));
  memcpy((char*)coef_blockSSE, (char*)coef_block, sizeof(coef_blockSSE));

  wsptr = (short *)workspaceSSE;
  outptr = (unsigned char*)workspaceSSE;

  
  row0 = _mm_load_si128((__m128i const*)(coef_blockSSE));
  row2 = _mm_load_si128((__m128i const*)(coef_blockSSE+8*2));
  row0 = _mm_mullo_epi16( row0, *(__m128i const*)quantptrSSE );
  row2 = _mm_mullo_epi16( row2, *(__m128i const*)(quantptrSSE+8*2) );

  iDCT_8_2ROWs(M128_tab_i_04, M128_tab_i_26);

  _mm_store_si128((__m128i*)(wsptr), row0);
  _mm_store_si128((__m128i*)(wsptr+8*2), row2);

  
  row0 = _mm_load_si128((__m128i const*)(coef_blockSSE+8*4));
  row2 = _mm_load_si128((__m128i const*)(coef_blockSSE+8*6));
  row0 = _mm_mullo_epi16(row0, *(__m128i const*)(quantptrSSE+8*4) );
  row2 = _mm_mullo_epi16(row2, *(__m128i const*)(quantptrSSE+8*6) );

  iDCT_8_2ROWs(M128_tab_i_04, M128_tab_i_26);

  _mm_store_si128((__m128i*)(wsptr+32), row0);
  _mm_store_si128((__m128i*)(wsptr+48), row2);

  
  row0 = _mm_load_si128((__m128i const*)(coef_blockSSE+8*3));
  row2 = _mm_load_si128((__m128i const*)(coef_blockSSE+8*1));
  row0 = _mm_mullo_epi16(row0, *(__m128i const*)(quantptrSSE+24) );
  row2 = _mm_mullo_epi16(row2, *(__m128i const*)(quantptrSSE+8) );

  iDCT_8_2ROWs(M128_tab_i_35, M128_tab_i_17);

  _mm_store_si128((__m128i*)(wsptr+24), row0);
  _mm_store_si128((__m128i*)(wsptr+8), row2);

  
  row0 = _mm_load_si128((__m128i const*)(coef_blockSSE+8*5));
  row2 = _mm_load_si128((__m128i const*)(coef_blockSSE+8*7));
  row0 = _mm_mullo_epi16(row0, *(__m128i const*)(quantptrSSE+40) );
  row2 = _mm_mullo_epi16(row2, *(__m128i const*)(quantptrSSE+56));

  iDCT_8_2ROWs( M128_tab_i_35, M128_tab_i_17);

  iDCT_8_COL();

  for(ctr = 0; ctr < DCTSIZE; ctr++)
  {
    outptrTemp = output_buf[ctr] + output_col;
    memcpy(outptrTemp, outptr, DCTSIZE);
    outptr += DCTSIZE;   
  }

  return;
}
#endif 
