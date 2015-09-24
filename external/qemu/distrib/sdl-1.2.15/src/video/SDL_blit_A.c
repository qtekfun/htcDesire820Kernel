/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2012 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "SDL_config.h"

#include "SDL_video.h"
#include "SDL_blit.h"


#if SDL_ASSEMBLY_ROUTINES
#  if defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
     
#    define MMX_ASMBLIT 0
#    define GCC_ASMBLIT 0
#  elif defined(_MSC_VER) && defined(_M_IX86)
#    if (_MSC_VER <= 1200)  
#      include <malloc.h>   
#      if defined(_mm_free)
#          define HAVE_MMINTRIN_H 1
#      endif
#    else  
#      define HAVE_MMINTRIN_H 1
#    endif
#    if HAVE_MMINTRIN_H
#      define MMX_ASMBLIT 1
#      define MSVC_ASMBLIT 1
#    endif
#  endif
#endif 

#include "SDL_cpuinfo.h"
#if GCC_ASMBLIT
#include "mmx.h"
#elif MSVC_ASMBLIT
#include <mmintrin.h>
#include <mm3dnow.h>
#endif


static void BlitNto1SurfaceAlpha(SDL_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	Uint8 *src = info->s_pixels;
	int srcskip = info->s_skip;
	Uint8 *dst = info->d_pixels;
	int dstskip = info->d_skip;
	Uint8 *palmap = info->table;
	SDL_PixelFormat *srcfmt = info->src;
	SDL_PixelFormat *dstfmt = info->dst;
	int srcbpp = srcfmt->BytesPerPixel;

	const unsigned A = srcfmt->alpha;

	while ( height-- ) {
	    DUFFS_LOOP4(
	    {
		Uint32 Pixel;
		unsigned sR;
		unsigned sG;
		unsigned sB;
		unsigned dR;
		unsigned dG;
		unsigned dB;
		DISEMBLE_RGB(src, srcbpp, srcfmt, Pixel, sR, sG, sB);
		dR = dstfmt->palette->colors[*dst].r;
		dG = dstfmt->palette->colors[*dst].g;
		dB = dstfmt->palette->colors[*dst].b;
		ALPHA_BLEND(sR, sG, sB, A, dR, dG, dB);
		dR &= 0xff;
		dG &= 0xff;
		dB &= 0xff;
		
		if ( palmap == NULL ) {
		    *dst =((dR>>5)<<(3+2))|
			  ((dG>>5)<<(2))|
			  ((dB>>6)<<(0));
		} else {
		    *dst = palmap[((dR>>5)<<(3+2))|
				  ((dG>>5)<<(2))  |
				  ((dB>>6)<<(0))];
		}
		dst++;
		src += srcbpp;
	    },
	    width);
	    src += srcskip;
	    dst += dstskip;
	}
}

static void BlitNto1PixelAlpha(SDL_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	Uint8 *src = info->s_pixels;
	int srcskip = info->s_skip;
	Uint8 *dst = info->d_pixels;
	int dstskip = info->d_skip;
	Uint8 *palmap = info->table;
	SDL_PixelFormat *srcfmt = info->src;
	SDL_PixelFormat *dstfmt = info->dst;
	int srcbpp = srcfmt->BytesPerPixel;

	
	while ( height-- ) {
	    DUFFS_LOOP4(
	    {
		Uint32 Pixel;
		unsigned sR;
		unsigned sG;
		unsigned sB;
		unsigned sA;
		unsigned dR;
		unsigned dG;
		unsigned dB;
		DISEMBLE_RGBA(src,srcbpp,srcfmt,Pixel,sR,sG,sB,sA);
		dR = dstfmt->palette->colors[*dst].r;
		dG = dstfmt->palette->colors[*dst].g;
		dB = dstfmt->palette->colors[*dst].b;
		ALPHA_BLEND(sR, sG, sB, sA, dR, dG, dB);
		dR &= 0xff;
		dG &= 0xff;
		dB &= 0xff;
		
		if ( palmap == NULL ) {
		    *dst =((dR>>5)<<(3+2))|
			  ((dG>>5)<<(2))|
			  ((dB>>6)<<(0));
		} else {
		    *dst = palmap[((dR>>5)<<(3+2))|
				  ((dG>>5)<<(2))  |
				  ((dB>>6)<<(0))  ];
		}
		dst++;
		src += srcbpp;
	    },
	    width);
	    src += srcskip;
	    dst += dstskip;
	}
}

static void BlitNto1SurfaceAlphaKey(SDL_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	Uint8 *src = info->s_pixels;
	int srcskip = info->s_skip;
	Uint8 *dst = info->d_pixels;
	int dstskip = info->d_skip;
	Uint8 *palmap = info->table;
	SDL_PixelFormat *srcfmt = info->src;
	SDL_PixelFormat *dstfmt = info->dst;
	int srcbpp = srcfmt->BytesPerPixel;
	Uint32 ckey = srcfmt->colorkey;

	const int A = srcfmt->alpha;

	while ( height-- ) {
	    DUFFS_LOOP(
	    {
		Uint32 Pixel;
		unsigned sR;
		unsigned sG;
		unsigned sB;
		unsigned dR;
		unsigned dG;
		unsigned dB;
		DISEMBLE_RGB(src, srcbpp, srcfmt, Pixel, sR, sG, sB);
		if ( Pixel != ckey ) {
		    dR = dstfmt->palette->colors[*dst].r;
		    dG = dstfmt->palette->colors[*dst].g;
		    dB = dstfmt->palette->colors[*dst].b;
		    ALPHA_BLEND(sR, sG, sB, A, dR, dG, dB);
		    dR &= 0xff;
		    dG &= 0xff;
		    dB &= 0xff;
		    
		    if ( palmap == NULL ) {
			*dst =((dR>>5)<<(3+2))|
			      ((dG>>5)<<(2)) |
			      ((dB>>6)<<(0));
		    } else {
			*dst = palmap[((dR>>5)<<(3+2))|
				      ((dG>>5)<<(2))  |
				      ((dB>>6)<<(0))  ];
		    }
		}
		dst++;
		src += srcbpp;
	    },
	    width);
	    src += srcskip;
	    dst += dstskip;
	}
}

#if GCC_ASMBLIT
static void BlitRGBtoRGBSurfaceAlpha128MMX(SDL_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	Uint32 *srcp = (Uint32 *)info->s_pixels;
	int srcskip = info->s_skip >> 2;
	Uint32 *dstp = (Uint32 *)info->d_pixels;
	int dstskip = info->d_skip >> 2;
	Uint32 dalpha = info->dst->Amask;
	Uint64 load;

	load = 0x00fefefe00fefefeULL;
	movq_m2r(load, mm4); 
	load = 0x0001010100010101ULL;
	movq_m2r(load, mm3); 
	movd_m2r(dalpha, mm7); 
	punpckldq_r2r(mm7, mm7); 
	while(height--) {
		DUFFS_LOOP_DOUBLE2(
		{
			Uint32 s = *srcp++;
			Uint32 d = *dstp;
			*dstp++ = ((((s & 0x00fefefe) + (d & 0x00fefefe)) >> 1)
				   + (s & d & 0x00010101)) | dalpha;
		},{
			movq_m2r((*dstp), mm2);
			movq_r2r(mm2, mm6); 

			movq_m2r((*srcp), mm1);
			movq_r2r(mm1, mm5); 

			pand_r2r(mm4, mm6); 
			pand_r2r(mm4, mm5); 
			paddd_r2r(mm6, mm5); 
			pand_r2r(mm1, mm2); 
			psrld_i2r(1, mm5); 
			pand_r2r(mm3, mm2); 
			paddd_r2r(mm5, mm2); 
			
			por_r2r(mm7, mm2); 
			movq_r2m(mm2, (*dstp));
			dstp += 2;
			srcp += 2;
		}, width);
		srcp += srcskip;
		dstp += dstskip;
	}
	emms();
}

static void BlitRGBtoRGBSurfaceAlphaMMX(SDL_BlitInfo *info)
{
	SDL_PixelFormat* df = info->dst;
	unsigned alpha = info->src->alpha;

	if (alpha == 128 && (df->Rmask | df->Gmask | df->Bmask) == 0x00FFFFFF) {
			
		BlitRGBtoRGBSurfaceAlpha128MMX(info);
	} else {
		int width = info->d_width;
		int height = info->d_height;
		Uint32 *srcp = (Uint32 *)info->s_pixels;
		int srcskip = info->s_skip >> 2;
		Uint32 *dstp = (Uint32 *)info->d_pixels;
		int dstskip = info->d_skip >> 2;

		pxor_r2r(mm5, mm5); 
		
		movd_m2r(alpha, mm4); 
		punpcklwd_r2r(mm4, mm4); 
		punpckldq_r2r(mm4, mm4); 
		alpha = (0xff << df->Rshift) | (0xff << df->Gshift) | (0xff << df->Bshift);
		movd_m2r(alpha, mm0); 
		punpcklbw_r2r(mm0, mm0); 
		pand_r2r(mm0, mm4); 
			
		movd_m2r(df->Amask, mm7); 
		punpckldq_r2r(mm7, mm7); 
		
		while(height--) {
			DUFFS_LOOP_DOUBLE2({
				
				movd_m2r((*srcp), mm1);
				movd_m2r((*dstp), mm2);
				punpcklbw_r2r(mm5, mm1); 
				punpcklbw_r2r(mm5, mm2); 

				psubw_r2r(mm2, mm1);
				pmullw_r2r(mm4, mm1); 
				psrlw_i2r(8, mm1); 
				paddb_r2r(mm1, mm2); 

				packuswb_r2r(mm5, mm2);  
				por_r2r(mm7, mm2); 
				movd_r2m(mm2, *dstp);
				++srcp;
				++dstp;
			},{
				
				movq_m2r((*srcp), mm0);
				movq_m2r((*dstp), mm2);
				movq_r2r(mm0, mm1); 
				movq_r2r(mm2, mm6); 

				punpcklbw_r2r(mm5, mm0); 
				punpckhbw_r2r(mm5, mm1); 
				punpcklbw_r2r(mm5, mm2); 
				punpckhbw_r2r(mm5, mm6); 

				psubw_r2r(mm2, mm0);
				pmullw_r2r(mm4, mm0); 
				psrlw_i2r(8, mm0); 
				paddb_r2r(mm0, mm2); 

				psubw_r2r(mm6, mm1);
				pmullw_r2r(mm4, mm1); 
				psrlw_i2r(8, mm1); 
				paddb_r2r(mm1, mm6); 

				packuswb_r2r(mm6, mm2);  
				por_r2r(mm7, mm2); 
				
				movq_r2m(mm2, *dstp);

  				srcp += 2;
  				dstp += 2;
  			}, width);
			srcp += srcskip;
			dstp += dstskip;
		}
		emms();
	}
}

static void BlitRGBtoRGBPixelAlphaMMX(SDL_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	Uint32 *srcp = (Uint32 *)info->s_pixels;
	int srcskip = info->s_skip >> 2;
	Uint32 *dstp = (Uint32 *)info->d_pixels;
	int dstskip = info->d_skip >> 2;
	SDL_PixelFormat* sf = info->src;
	Uint32 amask = sf->Amask;

	pxor_r2r(mm6, mm6); 
	
	movd_m2r(sf->Amask, mm7); 
	punpcklbw_r2r(mm7, mm7); 
	pcmpeqb_r2r(mm0, mm0); 
	movq_r2r(mm0, mm3); 
	pxor_r2r(mm0, mm7); 
	
	movq_r2r(mm7, mm0); 
	packsswb_r2r(mm6, mm0); 
	packsswb_r2r(mm6, mm3); 
	pxor_r2r(mm0, mm3); 
	
	__asm__ __volatile__ (
		"movd %0, %%mm5"
		: : "rm" ((Uint32) sf->Ashift) ); 

	while(height--) {
	    DUFFS_LOOP4({
		Uint32 alpha = *srcp & amask;
		if(alpha == 0) {
			
		} else if(alpha == amask) {
			
			
			movd_m2r((*srcp), mm1);
			movd_m2r((*dstp), mm2);
			pand_r2r(mm0, mm1); 
			pand_r2r(mm3, mm2); 
			por_r2r(mm1, mm2); 
			movd_r2m(mm2, (*dstp)); 
		} else {
			movd_m2r((*srcp), mm1);
			punpcklbw_r2r(mm6, mm1); 

			movd_m2r((*dstp), mm2);
			punpcklbw_r2r(mm6, mm2); 

			__asm__ __volatile__ (
				"movd %0, %%mm4"
				: : "r" (alpha) ); 
			psrld_r2r(mm5, mm4); 
			punpcklwd_r2r(mm4, mm4); 
			punpcklwd_r2r(mm4, mm4); 
			pand_r2r(mm7, mm4); 

					    
			psubw_r2r(mm2, mm1);
			pmullw_r2r(mm4, mm1); 
			psrlw_i2r(8, mm1); 
			paddb_r2r(mm1, mm2); 
			
			packuswb_r2r(mm6, mm2);  
			movd_r2m(mm2, *dstp);
		}
		++srcp;
		++dstp;
	    }, width);
	    srcp += srcskip;
	    dstp += dstskip;
	}
	emms();
}

#elif MSVC_ASMBLIT
static void BlitRGBtoRGBSurfaceAlpha128MMX(SDL_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	Uint32 *srcp = (Uint32 *)info->s_pixels;
	int srcskip = info->s_skip >> 2;
	Uint32 *dstp = (Uint32 *)info->d_pixels;
	int dstskip = info->d_skip >> 2;
	Uint32 dalpha = info->dst->Amask;

	__m64 src1, src2, dst1, dst2, lmask, hmask, dsta;
	
	hmask = _mm_set_pi32(0x00fefefe, 0x00fefefe); 
	lmask = _mm_set_pi32(0x00010101, 0x00010101); 
	dsta = _mm_set_pi32(dalpha, dalpha); 

	while (height--) {
		int n = width;
		if ( n & 1 ) {
			Uint32 s = *srcp++;
			Uint32 d = *dstp;
			*dstp++ = ((((s & 0x00fefefe) + (d & 0x00fefefe)) >> 1)
				   + (s & d & 0x00010101)) | dalpha;
			n--;
		}
		
		for (n >>= 1; n > 0; --n) {
			dst1 = *(__m64*)dstp; 
			dst2 = dst1;   

			src1 = *(__m64*)srcp; 
			src2 = src1; 

			dst2 = _mm_and_si64(dst2, hmask); 
			src2 = _mm_and_si64(src2, hmask); 
			src2 = _mm_add_pi32(src2, dst2); 
			src2 = _mm_srli_pi32(src2, 1); 

			dst1 = _mm_and_si64(dst1, src1); 
			dst1 = _mm_and_si64(dst1, lmask); 
			dst1 = _mm_add_pi32(dst1, src2); 
			dst1 = _mm_or_si64(dst1, dsta); 
			
			*(__m64*)dstp = dst1; 
			dstp += 2;
			srcp += 2;
		}
		
		srcp += srcskip;
		dstp += dstskip;
	}
	_mm_empty();
}

static void BlitRGBtoRGBSurfaceAlphaMMX(SDL_BlitInfo *info)
{
	SDL_PixelFormat* df = info->dst;
	Uint32 chanmask = df->Rmask | df->Gmask | df->Bmask;
	unsigned alpha = info->src->alpha;

	if (alpha == 128 && (df->Rmask | df->Gmask | df->Bmask) == 0x00FFFFFF) {
			
		BlitRGBtoRGBSurfaceAlpha128MMX(info);
	} else {
		int width = info->d_width;
		int height = info->d_height;
		Uint32 *srcp = (Uint32 *)info->s_pixels;
		int srcskip = info->s_skip >> 2;
		Uint32 *dstp = (Uint32 *)info->d_pixels;
		int dstskip = info->d_skip >> 2;
		Uint32 dalpha = df->Amask;
		Uint32 amult;

		__m64 src1, src2, dst1, dst2, mm_alpha, mm_zero, dsta;
		
		mm_zero = _mm_setzero_si64(); 
		
		amult = alpha | (alpha << 8);
		amult = amult | (amult << 16);
		chanmask = (0xff << df->Rshift) | (0xff << df->Gshift) | (0xff << df->Bshift);
		mm_alpha = _mm_set_pi32(0, amult & chanmask); 
		mm_alpha = _mm_unpacklo_pi8(mm_alpha, mm_zero); 
			
		dsta = _mm_set_pi32(dalpha, dalpha); 
		
		while (height--) {
			int n = width;
			if (n & 1) {
				
				src2 = _mm_cvtsi32_si64(*srcp); 
				src2 = _mm_unpacklo_pi8(src2, mm_zero); 

				dst1 = _mm_cvtsi32_si64(*dstp); 
				dst1 = _mm_unpacklo_pi8(dst1, mm_zero); 

				src2 = _mm_sub_pi16(src2, dst1); 
				src2 = _mm_mullo_pi16(src2, mm_alpha); 
				src2 = _mm_srli_pi16(src2, 8); 
				dst1 = _mm_add_pi8(src2, dst1); 
				
				dst1 = _mm_packs_pu16(dst1, mm_zero);  
				dst1 = _mm_or_si64(dst1, dsta); 
				*dstp = _mm_cvtsi64_si32(dst1); 

				++srcp;
				++dstp;
				
				n--;
			}

			for (n >>= 1; n > 0; --n) {
				
				src1 = *(__m64*)srcp; 
				src2 = src1; 
				src1 = _mm_unpacklo_pi8(src1, mm_zero); 
				src2 = _mm_unpackhi_pi8(src2, mm_zero); 

				dst1 = *(__m64*)dstp;
				dst2 = dst1; 
				dst1 = _mm_unpacklo_pi8(dst1, mm_zero); 
				dst2 = _mm_unpackhi_pi8(dst2, mm_zero); 

				src1 = _mm_sub_pi16(src1, dst1);
				src1 = _mm_mullo_pi16(src1, mm_alpha); 
				src1 = _mm_srli_pi16(src1, 8); 
				dst1 = _mm_add_pi8(src1, dst1); 

				src2 = _mm_sub_pi16(src2, dst2);
				src2 = _mm_mullo_pi16(src2, mm_alpha); 
				src2 = _mm_srli_pi16(src2, 8); 
				dst2 = _mm_add_pi8(src2, dst2); 
				
				dst1 = _mm_packs_pu16(dst1, dst2); 
				dst1 = _mm_or_si64(dst1, dsta); 

				*(__m64*)dstp = dst1; 

				srcp += 2;
				dstp += 2;
			}
			srcp += srcskip;
			dstp += dstskip;
		}
		_mm_empty();
	}
}

static void BlitRGBtoRGBPixelAlphaMMX(SDL_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	Uint32 *srcp = (Uint32 *)info->s_pixels;
	int srcskip = info->s_skip >> 2;
	Uint32 *dstp = (Uint32 *)info->d_pixels;
	int dstskip = info->d_skip >> 2;
	SDL_PixelFormat* sf = info->src;
	Uint32 chanmask = sf->Rmask | sf->Gmask | sf->Bmask;
	Uint32 amask = sf->Amask;
	Uint32 ashift = sf->Ashift;
	Uint64 multmask;

	__m64 src1, dst1, mm_alpha, mm_zero, dmask;

	mm_zero = _mm_setzero_si64(); 
	multmask = ~(0xFFFFi64 << (ashift * 2));
	dmask = *(__m64*) &multmask; 

	while(height--) {
		DUFFS_LOOP4({
		Uint32 alpha = *srcp & amask;
		if (alpha == 0) {
			
		} else if (alpha == amask) {
			
			*dstp = (*srcp & chanmask) | (*dstp & ~chanmask);
		} else {
			src1 = _mm_cvtsi32_si64(*srcp); 
			src1 = _mm_unpacklo_pi8(src1, mm_zero); 

			dst1 = _mm_cvtsi32_si64(*dstp); 
			dst1 = _mm_unpacklo_pi8(dst1, mm_zero); 

			mm_alpha = _mm_cvtsi32_si64(alpha); 
			mm_alpha = _mm_srli_si64(mm_alpha, ashift); 
			mm_alpha = _mm_unpacklo_pi16(mm_alpha, mm_alpha); 
			mm_alpha = _mm_unpacklo_pi32(mm_alpha, mm_alpha); 
			mm_alpha = _mm_and_si64(mm_alpha, dmask); 

					    
			src1 = _mm_sub_pi16(src1, dst1);
			src1 = _mm_mullo_pi16(src1, mm_alpha); 
			src1 = _mm_srli_pi16(src1, 8); 
			dst1 = _mm_add_pi8(src1, dst1); 
			dst1 = _mm_packs_pu16(dst1, mm_zero);  
			
			*dstp = _mm_cvtsi64_si32(dst1); 
		}
		++srcp;
		++dstp;
	    }, width);
	    srcp += srcskip;
	    dstp += dstskip;
	}
	_mm_empty();
}

#endif 

#if SDL_ALTIVEC_BLITTERS
#if __MWERKS__
#pragma altivec_model on
#endif
#if HAVE_ALTIVEC_H
#include <altivec.h>
#endif
#include <assert.h>

#if (defined(__MACOSX__) && (__GNUC__ < 4))
    #define VECUINT8_LITERAL(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) \
        (vector unsigned char) ( a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p )
    #define VECUINT16_LITERAL(a,b,c,d,e,f,g,h) \
        (vector unsigned short) ( a,b,c,d,e,f,g,h )
#else
    #define VECUINT8_LITERAL(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) \
        (vector unsigned char) { a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p }
    #define VECUINT16_LITERAL(a,b,c,d,e,f,g,h) \
        (vector unsigned short) { a,b,c,d,e,f,g,h }
#endif

#define UNALIGNED_PTR(x) (((size_t) x) & 0x0000000F)
#define VECPRINT(msg, v) do { \
    vector unsigned int tmpvec = (vector unsigned int)(v); \
    unsigned int *vp = (unsigned int *)&tmpvec; \
    printf("%s = %08X %08X %08X %08X\n", msg, vp[0], vp[1], vp[2], vp[3]); \
} while (0)

#define VEC_MERGE_PERMUTE() (vec_add(vec_lvsl(0, (int*)NULL), (vector unsigned char)vec_splat_u16(0x0F)))
#define VEC_U32_24() (vec_add(vec_splat_u32(12), vec_splat_u32(12)))
#define VEC_ALPHA_MASK() ((vector unsigned char)vec_sl((vector unsigned int)vec_splat_s8(-1), VEC_U32_24()))
#define VEC_ALIGNER(src) ((UNALIGNED_PTR(src)) \
    ? vec_lvsl(0, src) \
    : vec_add(vec_lvsl(8, src), vec_splat_u8(8)))

   
#define VEC_MULTIPLY_ALPHA(vs, vd, valpha, mergePermute, v1_16, v8_16) do { \
     \
    vector unsigned short vtemp1 = vec_mule(vs, valpha); \
     \
    vector unsigned short vtemp2 = vec_mulo(vs, valpha); \
     \
    vector unsigned char valpha2 = vec_nor(valpha, valpha); \
     \
    vector unsigned short vtemp3 = vec_mule(vd, valpha2); \
     \
    vector unsigned short vtemp4 = vec_mulo(vd, valpha2); \
     \
    vtemp1 = vec_add(vtemp1, vtemp3); \
    vtemp2 = vec_add(vtemp2, vtemp4); \
     \
    vtemp1 = vec_add(vtemp1, v1_16); \
    vtemp3 = vec_sr(vtemp1, v8_16); \
    vtemp1 = vec_add(vtemp1, vtemp3); \
     \
    vtemp2 = vec_add(vtemp2, v1_16); \
    vtemp4 = vec_sr(vtemp2, v8_16); \
    vtemp2 = vec_add(vtemp2, vtemp4); \
     \
    vd = (vector unsigned char)vec_perm(vtemp1, vtemp2, mergePermute); \
} while (0)
 
static vector unsigned char calc_swizzle32(const SDL_PixelFormat *srcfmt,
                                  const SDL_PixelFormat *dstfmt)
{
    
    const static struct SDL_PixelFormat default_pixel_format = {
        NULL, 0, 0,
        0, 0, 0, 0,
        16, 8, 0, 24,
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000,
        0, 0};
    if (!srcfmt) {
        srcfmt = &default_pixel_format;
    }
    if (!dstfmt) {
        dstfmt = &default_pixel_format;
    }
    const vector unsigned char plus = VECUINT8_LITERAL
                                            ( 0x00, 0x00, 0x00, 0x00,
                                              0x04, 0x04, 0x04, 0x04,
                                              0x08, 0x08, 0x08, 0x08,
                                              0x0C, 0x0C, 0x0C, 0x0C );
    vector unsigned char vswiz;
    vector unsigned int srcvec;
#define RESHIFT(X) (3 - ((X) >> 3))
    Uint32 rmask = RESHIFT(srcfmt->Rshift) << (dstfmt->Rshift);
    Uint32 gmask = RESHIFT(srcfmt->Gshift) << (dstfmt->Gshift);
    Uint32 bmask = RESHIFT(srcfmt->Bshift) << (dstfmt->Bshift);
    Uint32 amask;
    
    if (dstfmt->Amask) {
        amask = ((srcfmt->Amask) ? RESHIFT(srcfmt->Ashift) : 0x10) << (dstfmt->Ashift);
    } else {
        amask = 0x10101010 & ((dstfmt->Rmask | dstfmt->Gmask | dstfmt->Bmask) ^ 0xFFFFFFFF);
    }
#undef RESHIFT  
    ((unsigned int *)(char*)&srcvec)[0] = (rmask | gmask | bmask | amask);
    vswiz = vec_add(plus, (vector unsigned char)vec_splat(srcvec, 0));
    return(vswiz);
}

static void Blit32to565PixelAlphaAltivec(SDL_BlitInfo *info)
{
    int height = info->d_height;
    Uint8 *src = (Uint8 *)info->s_pixels;
    int srcskip = info->s_skip;
    Uint8 *dst = (Uint8 *)info->d_pixels;
    int dstskip = info->d_skip;
    SDL_PixelFormat *srcfmt = info->src;

    vector unsigned char v0 = vec_splat_u8(0);
    vector unsigned short v8_16 = vec_splat_u16(8);
    vector unsigned short v1_16 = vec_splat_u16(1);
    vector unsigned short v2_16 = vec_splat_u16(2);
    vector unsigned short v3_16 = vec_splat_u16(3);
    vector unsigned int v8_32 = vec_splat_u32(8);
    vector unsigned int v16_32 = vec_add(v8_32, v8_32);
    vector unsigned short v3f = VECUINT16_LITERAL(
        0x003f, 0x003f, 0x003f, 0x003f,
        0x003f, 0x003f, 0x003f, 0x003f);
    vector unsigned short vfc = VECUINT16_LITERAL(
        0x00fc, 0x00fc, 0x00fc, 0x00fc,
        0x00fc, 0x00fc, 0x00fc, 0x00fc);

    vector unsigned char vredalpha1 = VECUINT8_LITERAL(
        0x10, 0x00, 0x01, 0x01,
        0x10, 0x02, 0x01, 0x01,
        0x10, 0x04, 0x01, 0x01,
        0x10, 0x06, 0x01, 0x01
    );
    vector unsigned char vredalpha2 = (vector unsigned char)(
        vec_add((vector unsigned int)vredalpha1, vec_sl(v8_32, v16_32))
    );
    vector unsigned char vblue1 = VECUINT8_LITERAL(
        0x00, 0x01, 0x02, 0x11,
        0x04, 0x05, 0x06, 0x13,
        0x08, 0x09, 0x0a, 0x15,
        0x0c, 0x0d, 0x0e, 0x17
    );
    vector unsigned char vblue2 = (vector unsigned char)(
        vec_add((vector unsigned int)vblue1, v8_32)
    );
    vector unsigned char vgreen1 = VECUINT8_LITERAL(
        0x00, 0x01, 0x10, 0x03,
        0x04, 0x05, 0x12, 0x07,
        0x08, 0x09, 0x14, 0x0b,
        0x0c, 0x0d, 0x16, 0x0f
    );
    vector unsigned char vgreen2 = (vector unsigned char)(
        vec_add((vector unsigned int)vgreen1, vec_sl(v8_32, v8_32))
    );
    vector unsigned char vgmerge = VECUINT8_LITERAL(
        0x00, 0x02, 0x00, 0x06,
        0x00, 0x0a, 0x00, 0x0e,
        0x00, 0x12, 0x00, 0x16,
        0x00, 0x1a, 0x00, 0x1e);
    vector unsigned char mergePermute = VEC_MERGE_PERMUTE();
    vector unsigned char vpermute = calc_swizzle32(srcfmt, NULL);
    vector unsigned char valphaPermute = vec_and(vec_lvsl(0, (int *)NULL), vec_splat_u8(0xC));

    vector unsigned short vf800 = (vector unsigned short)vec_splat_u8(-7);
    vf800 = vec_sl(vf800, vec_splat_u16(8));

    while(height--) {
        int extrawidth;
        vector unsigned char valigner;
        vector unsigned char vsrc;
        vector unsigned char voverflow;
        int width = info->d_width;

#define ONE_PIXEL_BLEND(condition, widthvar) \
        while (condition) { \
            Uint32 Pixel; \
            unsigned sR, sG, sB, dR, dG, dB, sA; \
            DISEMBLE_RGBA(src, 4, srcfmt, Pixel, sR, sG, sB, sA); \
            if(sA) { \
                unsigned short dstpixel = *((unsigned short *)dst); \
                dR = (dstpixel >> 8) & 0xf8; \
                dG = (dstpixel >> 3) & 0xfc; \
                dB = (dstpixel << 3) & 0xf8; \
                ALPHA_BLEND(sR, sG, sB, sA, dR, dG, dB); \
                *((unsigned short *)dst) = ( \
                    ((dR & 0xf8) << 8) | ((dG & 0xfc) << 3) | (dB >> 3) \
                ); \
            } \
            src += 4; \
            dst += 2; \
            widthvar--; \
        }
        ONE_PIXEL_BLEND((UNALIGNED_PTR(dst)) && (width), width);
        extrawidth = (width % 8);
        valigner = VEC_ALIGNER(src);
        vsrc = (vector unsigned char)vec_ld(0, src);
        width -= extrawidth;
        while (width) {
            vector unsigned char valpha;
            vector unsigned char vsrc1, vsrc2;
            vector unsigned char vdst1, vdst2;
            vector unsigned short vR, vG, vB;
            vector unsigned short vpixel, vrpixel, vgpixel, vbpixel;

            
            voverflow = (vector unsigned char)vec_ld(15, src);
            vsrc = vec_perm(vsrc, voverflow, valigner);
            vsrc1 = vec_perm(vsrc, vsrc, vpermute);
            src += 16;
            vsrc = (vector unsigned char)vec_ld(15, src);
            voverflow = vec_perm(voverflow, vsrc, valigner);
            vsrc2 = vec_perm(voverflow, voverflow, vpermute);
            src += 16;

            
            voverflow = vec_ld(0, dst);
            vR = vec_and((vector unsigned short)voverflow, vf800);
            vB = vec_sl((vector unsigned short)voverflow, v3_16);
            vG = vec_sl(vB, v2_16);
            vdst1 = (vector unsigned char)vec_perm((vector unsigned char)vR, (vector unsigned char)vR, vredalpha1);
            vdst1 = vec_perm(vdst1, (vector unsigned char)vB, vblue1);
            vdst1 = vec_perm(vdst1, (vector unsigned char)vG, vgreen1);
            vdst2 = (vector unsigned char)vec_perm((vector unsigned char)vR, (vector unsigned char)vR, vredalpha2);
            vdst2 = vec_perm(vdst2, (vector unsigned char)vB, vblue2);
            vdst2 = vec_perm(vdst2, (vector unsigned char)vG, vgreen2);

            
            valpha = vec_perm(vsrc1, v0, valphaPermute);
            VEC_MULTIPLY_ALPHA(vsrc1, vdst1, valpha, mergePermute, v1_16, v8_16);
            valpha = vec_perm(vsrc2, v0, valphaPermute);
            VEC_MULTIPLY_ALPHA(vsrc2, vdst2, valpha, mergePermute, v1_16, v8_16);

            
            vpixel = (vector unsigned short)vec_packpx((vector unsigned int)vdst1, (vector unsigned int)vdst2);
            vgpixel = (vector unsigned short)vec_perm(vdst1, vdst2, vgmerge);
            vgpixel = vec_and(vgpixel, vfc);
            vgpixel = vec_sl(vgpixel, v3_16);
            vrpixel = vec_sl(vpixel, v1_16);
            vrpixel = vec_and(vrpixel, vf800);
            vbpixel = vec_and(vpixel, v3f);
            vdst1 = vec_or((vector unsigned char)vrpixel, (vector unsigned char)vgpixel);
            vdst1 = vec_or(vdst1, (vector unsigned char)vbpixel);
            
            
            vec_st(vdst1, 0, dst);

            width -= 8;
            dst += 16;
        }
        ONE_PIXEL_BLEND((extrawidth), extrawidth);
#undef ONE_PIXEL_BLEND
        src += srcskip;
        dst += dstskip;
    }
}

static void Blit32to32SurfaceAlphaKeyAltivec(SDL_BlitInfo *info)
{
    unsigned alpha = info->src->alpha;
    int height = info->d_height;
    Uint32 *srcp = (Uint32 *)info->s_pixels;
    int srcskip = info->s_skip >> 2;
    Uint32 *dstp = (Uint32 *)info->d_pixels;
    int dstskip = info->d_skip >> 2;
    SDL_PixelFormat *srcfmt = info->src;
    SDL_PixelFormat *dstfmt = info->dst;
    unsigned sA = srcfmt->alpha;
    unsigned dA = dstfmt->Amask ? SDL_ALPHA_OPAQUE : 0;
    Uint32 rgbmask = srcfmt->Rmask | srcfmt->Gmask | srcfmt->Bmask;
    Uint32 ckey = info->src->colorkey;
    vector unsigned char mergePermute;
    vector unsigned char vsrcPermute;
    vector unsigned char vdstPermute;
    vector unsigned char vsdstPermute;
    vector unsigned char valpha;
    vector unsigned char valphamask;
    vector unsigned char vbits;
    vector unsigned char v0;
    vector unsigned short v1;
    vector unsigned short v8;
    vector unsigned int vckey;
    vector unsigned int vrgbmask;

    mergePermute = VEC_MERGE_PERMUTE();
    v0 = vec_splat_u8(0);
    v1 = vec_splat_u16(1);
    v8 = vec_splat_u16(8);

    
    valphamask = VEC_ALPHA_MASK();

    vsrcPermute = calc_swizzle32(srcfmt, NULL);
    vdstPermute = calc_swizzle32(NULL, dstfmt);
    vsdstPermute = calc_swizzle32(dstfmt, NULL);

    
    ((unsigned char *)&valpha)[0] = alpha;
    valpha = vec_splat(valpha, 0);
    vbits = (vector unsigned char)vec_splat_s8(-1);

    ckey &= rgbmask;
    ((unsigned int *)(char*)&vckey)[0] = ckey;
    vckey = vec_splat(vckey, 0);
    ((unsigned int *)(char*)&vrgbmask)[0] = rgbmask;
    vrgbmask = vec_splat(vrgbmask, 0);

    while(height--) {
        int width = info->d_width;
#define ONE_PIXEL_BLEND(condition, widthvar) \
        while (condition) { \
            Uint32 Pixel; \
            unsigned sR, sG, sB, dR, dG, dB; \
            RETRIEVE_RGB_PIXEL(((Uint8 *)srcp), 4, Pixel); \
            if(sA && Pixel != ckey) { \
                RGB_FROM_PIXEL(Pixel, srcfmt, sR, sG, sB); \
                DISEMBLE_RGB(((Uint8 *)dstp), 4, dstfmt, Pixel, dR, dG, dB); \
                ALPHA_BLEND(sR, sG, sB, sA, dR, dG, dB); \
                ASSEMBLE_RGBA(((Uint8 *)dstp), 4, dstfmt, dR, dG, dB, dA); \
            } \
            dstp++; \
            srcp++; \
            widthvar--; \
        }
        ONE_PIXEL_BLEND((UNALIGNED_PTR(dstp)) && (width), width);
        if (width > 0) {
            int extrawidth = (width % 4);
            vector unsigned char valigner = VEC_ALIGNER(srcp);
            vector unsigned char vs = (vector unsigned char)vec_ld(0, srcp);
            width -= extrawidth;
            while (width) {
                vector unsigned char vsel;
                vector unsigned char voverflow;
                vector unsigned char vd;
                vector unsigned char vd_orig;

                
                voverflow = (vector unsigned char)vec_ld(15, srcp);
                vs = vec_perm(vs, voverflow, valigner);
                
                
                vsel = (vector unsigned char)vec_and((vector unsigned int)vs, vrgbmask);
                vsel = (vector unsigned char)vec_cmpeq((vector unsigned int)vsel, vckey);

                
                vs = vec_perm(vs, valpha, vsrcPermute);

                
                vd = (vector unsigned char)vec_ld(0, dstp);
                vd_orig = vd = vec_perm(vd, v0, vsdstPermute);

                VEC_MULTIPLY_ALPHA(vs, vd, valpha, mergePermute, v1, v8);

                
                vd = vec_or(vd, valphamask);

                
                vd = vec_sel(vd, vd_orig, vsel);
                
                
                vd = vec_perm(vd, vbits, vdstPermute);

                
                vec_st((vector unsigned int)vd, 0, dstp);
                
                srcp += 4;
                dstp += 4;
                width -= 4;
                vs = voverflow;
            }
            ONE_PIXEL_BLEND((extrawidth), extrawidth);
        }
#undef ONE_PIXEL_BLEND
 
        srcp += srcskip;
        dstp += dstskip;
    }
}


static void Blit32to32PixelAlphaAltivec(SDL_BlitInfo *info)
{
    int width = info->d_width;
    int height = info->d_height;
    Uint32 *srcp = (Uint32 *)info->s_pixels;
    int srcskip = info->s_skip >> 2;
    Uint32 *dstp = (Uint32 *)info->d_pixels;
    int dstskip = info->d_skip >> 2;
    SDL_PixelFormat *srcfmt = info->src;
    SDL_PixelFormat *dstfmt = info->dst;
    vector unsigned char mergePermute;
    vector unsigned char valphaPermute;
    vector unsigned char vsrcPermute;
    vector unsigned char vdstPermute;
    vector unsigned char vsdstPermute;
    vector unsigned char valphamask;
    vector unsigned char vpixelmask;
    vector unsigned char v0;
    vector unsigned short v1;
    vector unsigned short v8;

    v0 = vec_splat_u8(0);
    v1 = vec_splat_u16(1);
    v8 = vec_splat_u16(8);
    mergePermute = VEC_MERGE_PERMUTE();
    valphamask = VEC_ALPHA_MASK();
    valphaPermute = vec_and(vec_lvsl(0, (int *)NULL), vec_splat_u8(0xC));
    vpixelmask = vec_nor(valphamask, v0);
    vsrcPermute = calc_swizzle32(srcfmt, NULL);
    vdstPermute = calc_swizzle32(NULL, dstfmt);
    vsdstPermute = calc_swizzle32(dstfmt, NULL);

	while ( height-- ) {
        width = info->d_width;
#define ONE_PIXEL_BLEND(condition, widthvar) while ((condition)) { \
            Uint32 Pixel; \
            unsigned sR, sG, sB, dR, dG, dB, sA, dA; \
            DISEMBLE_RGBA((Uint8 *)srcp, 4, srcfmt, Pixel, sR, sG, sB, sA); \
            if(sA) { \
              DISEMBLE_RGBA((Uint8 *)dstp, 4, dstfmt, Pixel, dR, dG, dB, dA); \
              ALPHA_BLEND(sR, sG, sB, sA, dR, dG, dB); \
              ASSEMBLE_RGBA((Uint8 *)dstp, 4, dstfmt, dR, dG, dB, dA); \
            } \
            ++srcp; \
            ++dstp; \
            widthvar--; \
        }
        ONE_PIXEL_BLEND((UNALIGNED_PTR(dstp)) && (width), width);
        if (width > 0) {
            
            
            int extrawidth = (width % 4);
            vector unsigned char valigner = VEC_ALIGNER(srcp);
            vector unsigned char vs = (vector unsigned char)vec_ld(0, srcp);
            width -= extrawidth;
            while (width) {
                vector unsigned char voverflow;
                vector unsigned char vd;
                vector unsigned char valpha;
                vector unsigned char vdstalpha;
                
                voverflow = (vector unsigned char)vec_ld(15, srcp);
                vs = vec_perm(vs, voverflow, valigner);
                vs = vec_perm(vs, v0, vsrcPermute);

                valpha = vec_perm(vs, v0, valphaPermute);
                
                
                vd = (vector unsigned char)vec_ld(0, dstp);
                vd = vec_perm(vd, v0, vsdstPermute);
                vdstalpha = vec_and(vd, valphamask);

                VEC_MULTIPLY_ALPHA(vs, vd, valpha, mergePermute, v1, v8);

                
                vd = vec_and(vd, vpixelmask);
                vd = vec_or(vd, vdstalpha);
                vd = vec_perm(vd, v0, vdstPermute);

                
                vec_st((vector unsigned int)vd, 0, dstp);
                
                srcp += 4;
                dstp += 4;
                width -= 4;
                vs = voverflow;

            }
            ONE_PIXEL_BLEND((extrawidth), extrawidth);
        }
	    srcp += srcskip;
	    dstp += dstskip;
#undef ONE_PIXEL_BLEND
	}
}

static void BlitRGBtoRGBPixelAlphaAltivec(SDL_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	Uint32 *srcp = (Uint32 *)info->s_pixels;
	int srcskip = info->s_skip >> 2;
	Uint32 *dstp = (Uint32 *)info->d_pixels;
	int dstskip = info->d_skip >> 2;
    vector unsigned char mergePermute;
    vector unsigned char valphaPermute;
    vector unsigned char valphamask;
    vector unsigned char vpixelmask;
    vector unsigned char v0;
    vector unsigned short v1;
    vector unsigned short v8;
    v0 = vec_splat_u8(0);
    v1 = vec_splat_u16(1);
    v8 = vec_splat_u16(8);
    mergePermute = VEC_MERGE_PERMUTE();
    valphamask = VEC_ALPHA_MASK();
    valphaPermute = vec_and(vec_lvsl(0, (int *)NULL), vec_splat_u8(0xC));
    
 
    vpixelmask = vec_nor(valphamask, v0);
	while(height--) {
        width = info->d_width;
#define ONE_PIXEL_BLEND(condition, widthvar) \
        while ((condition)) { \
            Uint32 dalpha; \
            Uint32 d; \
            Uint32 s1; \
            Uint32 d1; \
            Uint32 s = *srcp; \
            Uint32 alpha = s >> 24; \
            if(alpha) { \
              if(alpha == SDL_ALPHA_OPAQUE) { \
                *dstp = (s & 0x00ffffff) | (*dstp & 0xff000000); \
              } else { \
                d = *dstp; \
                dalpha = d & 0xff000000; \
                s1 = s & 0xff00ff; \
                d1 = d & 0xff00ff; \
                d1 = (d1 + ((s1 - d1) * alpha >> 8)) & 0xff00ff; \
                s &= 0xff00; \
                d &= 0xff00; \
                d = (d + ((s - d) * alpha >> 8)) & 0xff00; \
                *dstp = d1 | d | dalpha; \
              } \
            } \
            ++srcp; \
            ++dstp; \
            widthvar--; \
	    }
        ONE_PIXEL_BLEND((UNALIGNED_PTR(dstp)) && (width), width);
        if (width > 0) {
            int extrawidth = (width % 4);
            vector unsigned char valigner = VEC_ALIGNER(srcp);
            vector unsigned char vs = (vector unsigned char)vec_ld(0, srcp);
            width -= extrawidth;
            while (width) {
                vector unsigned char voverflow;
                vector unsigned char vd;
                vector unsigned char valpha;
                vector unsigned char vdstalpha;
                
                voverflow = (vector unsigned char)vec_ld(15, srcp);
                vs = vec_perm(vs, voverflow, valigner);

                valpha = vec_perm(vs, v0, valphaPermute);
                
                
                vd = (vector unsigned char)vec_ld(0, dstp);
                vdstalpha = vec_and(vd, valphamask);

                VEC_MULTIPLY_ALPHA(vs, vd, valpha, mergePermute, v1, v8);

                
                vd = vec_and(vd, vpixelmask);
                vd = vec_or(vd, vdstalpha);

                
                vec_st((vector unsigned int)vd, 0, dstp);
                
                srcp += 4;
                dstp += 4;
                width -= 4;
                vs = voverflow;
            }
            ONE_PIXEL_BLEND((extrawidth), extrawidth);
        }
	    srcp += srcskip;
	    dstp += dstskip;
	}
#undef ONE_PIXEL_BLEND
}

static void Blit32to32SurfaceAlphaAltivec(SDL_BlitInfo *info)
{
    
	unsigned alpha = info->src->alpha;
    int height = info->d_height;
    Uint32 *srcp = (Uint32 *)info->s_pixels;
    int srcskip = info->s_skip >> 2;
    Uint32 *dstp = (Uint32 *)info->d_pixels;
    int dstskip = info->d_skip >> 2;
    SDL_PixelFormat *srcfmt = info->src;
    SDL_PixelFormat *dstfmt = info->dst;
	unsigned sA = srcfmt->alpha;
	unsigned dA = dstfmt->Amask ? SDL_ALPHA_OPAQUE : 0;
    vector unsigned char mergePermute;
    vector unsigned char vsrcPermute;
    vector unsigned char vdstPermute;
    vector unsigned char vsdstPermute;
    vector unsigned char valpha;
    vector unsigned char valphamask;
    vector unsigned char vbits;
    vector unsigned short v1;
    vector unsigned short v8;

    mergePermute = VEC_MERGE_PERMUTE();
    v1 = vec_splat_u16(1);
    v8 = vec_splat_u16(8);

    
    valphamask = VEC_ALPHA_MASK();

    vsrcPermute = calc_swizzle32(srcfmt, NULL);
    vdstPermute = calc_swizzle32(NULL, dstfmt);
    vsdstPermute = calc_swizzle32(dstfmt, NULL);

    
    ((unsigned char *)&valpha)[0] = alpha;
    valpha = vec_splat(valpha, 0);
    vbits = (vector unsigned char)vec_splat_s8(-1);

    while(height--) {
        int width = info->d_width;
#define ONE_PIXEL_BLEND(condition, widthvar) while ((condition)) { \
            Uint32 Pixel; \
            unsigned sR, sG, sB, dR, dG, dB; \
            DISEMBLE_RGB(((Uint8 *)srcp), 4, srcfmt, Pixel, sR, sG, sB); \
            DISEMBLE_RGB(((Uint8 *)dstp), 4, dstfmt, Pixel, dR, dG, dB); \
            ALPHA_BLEND(sR, sG, sB, sA, dR, dG, dB); \
            ASSEMBLE_RGBA(((Uint8 *)dstp), 4, dstfmt, dR, dG, dB, dA); \
            ++srcp; \
            ++dstp; \
            widthvar--; \
        }
        ONE_PIXEL_BLEND((UNALIGNED_PTR(dstp)) && (width), width);
        if (width > 0) {
            int extrawidth = (width % 4);
            vector unsigned char valigner = VEC_ALIGNER(srcp);
            vector unsigned char vs = (vector unsigned char)vec_ld(0, srcp);
            width -= extrawidth;
            while (width) {
                vector unsigned char voverflow;
                vector unsigned char vd;

                
                voverflow = (vector unsigned char)vec_ld(15, srcp);
                vs = vec_perm(vs, voverflow, valigner);
                vs = vec_perm(vs, valpha, vsrcPermute);
                
                
                vd = (vector unsigned char)vec_ld(0, dstp);
                vd = vec_perm(vd, vd, vsdstPermute);

                VEC_MULTIPLY_ALPHA(vs, vd, valpha, mergePermute, v1, v8);

                
                vd = vec_or(vd, valphamask);
                vd = vec_perm(vd, vbits, vdstPermute);

                
                vec_st((vector unsigned int)vd, 0, dstp);
                
                srcp += 4;
                dstp += 4;
                width -= 4;
                vs = voverflow;
            }
            ONE_PIXEL_BLEND((extrawidth), extrawidth);
        }
#undef ONE_PIXEL_BLEND
 
        srcp += srcskip;
        dstp += dstskip;
    }

}


static void BlitRGBtoRGBSurfaceAlphaAltivec(SDL_BlitInfo *info)
{
	unsigned alpha = info->src->alpha;
    int height = info->d_height;
    Uint32 *srcp = (Uint32 *)info->s_pixels;
    int srcskip = info->s_skip >> 2;
    Uint32 *dstp = (Uint32 *)info->d_pixels;
    int dstskip = info->d_skip >> 2;
    vector unsigned char mergePermute;
    vector unsigned char valpha;
    vector unsigned char valphamask;
    vector unsigned short v1;
    vector unsigned short v8;

    mergePermute = VEC_MERGE_PERMUTE();
    v1 = vec_splat_u16(1);
    v8 = vec_splat_u16(8);

    
    valphamask = VEC_ALPHA_MASK();

    
    ((unsigned char *)&valpha)[0] = alpha;
    valpha = vec_splat(valpha, 0);

    while(height--) {
        int width = info->d_width;
#define ONE_PIXEL_BLEND(condition, widthvar) while ((condition)) { \
            Uint32 s = *srcp; \
            Uint32 d = *dstp; \
            Uint32 s1 = s & 0xff00ff; \
            Uint32 d1 = d & 0xff00ff; \
            d1 = (d1 + ((s1 - d1) * alpha >> 8)) \
                 & 0xff00ff; \
            s &= 0xff00; \
            d &= 0xff00; \
            d = (d + ((s - d) * alpha >> 8)) & 0xff00; \
            *dstp = d1 | d | 0xff000000; \
            ++srcp; \
            ++dstp; \
            widthvar--; \
        }
        ONE_PIXEL_BLEND((UNALIGNED_PTR(dstp)) && (width), width);
        if (width > 0) {
            int extrawidth = (width % 4);
            vector unsigned char valigner = VEC_ALIGNER(srcp);
            vector unsigned char vs = (vector unsigned char)vec_ld(0, srcp);
            width -= extrawidth;
            while (width) {
                vector unsigned char voverflow;
                vector unsigned char vd;

                
                voverflow = (vector unsigned char)vec_ld(15, srcp);
                vs = vec_perm(vs, voverflow, valigner);
                
                
                vd = (vector unsigned char)vec_ld(0, dstp);

                VEC_MULTIPLY_ALPHA(vs, vd, valpha, mergePermute, v1, v8);

                
                vd = vec_or(vd, valphamask);

                
                vec_st((vector unsigned int)vd, 0, dstp);
                
                srcp += 4;
                dstp += 4;
                width -= 4;
                vs = voverflow;
            }
            ONE_PIXEL_BLEND((extrawidth), extrawidth);
        }
#undef ONE_PIXEL_BLEND
 
        srcp += srcskip;
        dstp += dstskip;
    }
}
#if __MWERKS__
#pragma altivec_model off
#endif
#endif 

static void BlitRGBtoRGBSurfaceAlpha128(SDL_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	Uint32 *srcp = (Uint32 *)info->s_pixels;
	int srcskip = info->s_skip >> 2;
	Uint32 *dstp = (Uint32 *)info->d_pixels;
	int dstskip = info->d_skip >> 2;

	while(height--) {
	    DUFFS_LOOP4({
		    Uint32 s = *srcp++;
		    Uint32 d = *dstp;
		    *dstp++ = ((((s & 0x00fefefe) + (d & 0x00fefefe)) >> 1)
			       + (s & d & 0x00010101)) | 0xff000000;
	    }, width);
	    srcp += srcskip;
	    dstp += dstskip;
	}
}

static void BlitRGBtoRGBSurfaceAlpha(SDL_BlitInfo *info)
{
	unsigned alpha = info->src->alpha;
	if(alpha == 128) {
		BlitRGBtoRGBSurfaceAlpha128(info);
	} else {
		int width = info->d_width;
		int height = info->d_height;
		Uint32 *srcp = (Uint32 *)info->s_pixels;
		int srcskip = info->s_skip >> 2;
		Uint32 *dstp = (Uint32 *)info->d_pixels;
		int dstskip = info->d_skip >> 2;
		Uint32 s;
		Uint32 d;
		Uint32 s1;
		Uint32 d1;

		while(height--) {
			DUFFS_LOOP_DOUBLE2({
				
				s = *srcp;
				d = *dstp;
				s1 = s & 0xff00ff;
				d1 = d & 0xff00ff;
				d1 = (d1 + ((s1 - d1) * alpha >> 8))
				     & 0xff00ff;
				s &= 0xff00;
				d &= 0xff00;
				d = (d + ((s - d) * alpha >> 8)) & 0xff00;
				*dstp = d1 | d | 0xff000000;
				++srcp;
				++dstp;
			},{
			        
				s = *srcp;
				d = *dstp;
				s1 = s & 0xff00ff;
				d1 = d & 0xff00ff;
				d1 += (s1 - d1) * alpha >> 8;
				d1 &= 0xff00ff;
				     
				s = ((s & 0xff00) >> 8) | 
					((srcp[1] & 0xff00) << 8);
				d = ((d & 0xff00) >> 8) |
					((dstp[1] & 0xff00) << 8);
				d += (s - d) * alpha >> 8;
				d &= 0x00ff00ff;
				
				*dstp++ = d1 | ((d << 8) & 0xff00) | 0xff000000;
				++srcp;
				
			        s1 = *srcp;
				d1 = *dstp;
				s1 &= 0xff00ff;
				d1 &= 0xff00ff;
				d1 += (s1 - d1) * alpha >> 8;
				d1 &= 0xff00ff;
				
				*dstp = d1 | ((d >> 8) & 0xff00) | 0xff000000;
				++srcp;
				++dstp;
			}, width);
			srcp += srcskip;
			dstp += dstskip;
		}
	}
}

static void BlitRGBtoRGBPixelAlpha(SDL_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	Uint32 *srcp = (Uint32 *)info->s_pixels;
	int srcskip = info->s_skip >> 2;
	Uint32 *dstp = (Uint32 *)info->d_pixels;
	int dstskip = info->d_skip >> 2;

	while(height--) {
	    DUFFS_LOOP4({
		Uint32 dalpha;
		Uint32 d;
		Uint32 s1;
		Uint32 d1;
		Uint32 s = *srcp;
		Uint32 alpha = s >> 24;
		if(alpha) {   
		  if(alpha == SDL_ALPHA_OPAQUE) {
		    *dstp = (s & 0x00ffffff) | (*dstp & 0xff000000);
		  } else {
		    d = *dstp;
		    dalpha = d & 0xff000000;
		    s1 = s & 0xff00ff;
		    d1 = d & 0xff00ff;
		    d1 = (d1 + ((s1 - d1) * alpha >> 8)) & 0xff00ff;
		    s &= 0xff00;
		    d &= 0xff00;
		    d = (d + ((s - d) * alpha >> 8)) & 0xff00;
		    *dstp = d1 | d | dalpha;
		  }
		}
		++srcp;
		++dstp;
	    }, width);
	    srcp += srcskip;
	    dstp += dstskip;
	}
}

#if GCC_ASMBLIT
static void BlitRGBtoRGBPixelAlphaMMX3DNOW(SDL_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	Uint32 *srcp = (Uint32 *)info->s_pixels;
	int srcskip = info->s_skip >> 2;
	Uint32 *dstp = (Uint32 *)info->d_pixels;
	int dstskip = info->d_skip >> 2;
	SDL_PixelFormat* sf = info->src;
	Uint32 amask = sf->Amask;

	__asm__ (
	
	"pxor       %%mm6, %%mm6\n"
	
	
	"movd      %0, %%mm7\n\t"           
	"punpcklbw %%mm7, %%mm7\n\t"        
	"pcmpeqb   %%mm4, %%mm4\n\t"        
	"movq      %%mm4, %%mm3\n\t"        
	"pxor      %%mm4, %%mm7\n\t"        

	
	"movq      %%mm7, %%mm4\n\t"        
	"packsswb  %%mm6, %%mm4\n\t"        
	"packsswb  %%mm6, %%mm3\n\t"        
	"pxor      %%mm4, %%mm3\n\t"        
	
	
	"movd      %1, %%mm5\n\t" 

	  :  : "rm" (amask), "rm" ((Uint32) sf->Ashift) );

	while(height--) {

	    DUFFS_LOOP4({
		Uint32 alpha;

		__asm__ (
		"prefetch 64(%0)\n"
		"prefetch 64(%1)\n"
			: : "r" (srcp), "r" (dstp) );

		alpha = *srcp & amask;
		if(alpha == 0) {
		    
		}
		else if(alpha == amask) {
			
		    
			    __asm__ (
		    "movd      (%0),  %%mm0\n\t" 
		    "movd      (%1),  %%mm1\n\t" 
		    "pand      %%mm4, %%mm0\n\t" 
		    "pand      %%mm3, %%mm1\n\t" 
		    "por       %%mm0, %%mm1\n\t" 
		    "movd      %%mm1, (%1) \n\t" 

		     : : "r" (srcp), "r" (dstp) );
		} 

		else {
			    __asm__ (
		    
		    "movd      (%0), %%mm0\n"		    
		    "movd      (%1), %%mm1\n"		    

		    

		    
		     
		    
		    
		    
		    "movd       %2,    %%mm2\n"
		    "psrld      %%mm5, %%mm2\n"                
		    "punpcklwd	%%mm2, %%mm2\n"	            
		    "punpckldq	%%mm2, %%mm2\n"             
		    "pand       %%mm7, %%mm2\n"              

		    
		    "punpcklbw %%mm6, %%mm0\n"		    
		    "punpcklbw %%mm6, %%mm1\n"              

		    
		    "psubw    %%mm1, %%mm0\n"		    

		    
		    "pmullw    %%mm2, %%mm0\n"		    
		    "psrlw     $8,    %%mm0\n"		    
		    "paddb     %%mm1, %%mm0\n"		    

		    "packuswb  %%mm0, %%mm0\n"              
		    
		    "movd      %%mm0, (%1)\n"               

		     : : "r" (srcp), "r" (dstp), "r" (alpha) );

		}
		++srcp;
		++dstp;
	    }, width);
	    srcp += srcskip;
	    dstp += dstskip;
	}

	__asm__ (
	"emms\n"
		:   );
}

#elif MSVC_ASMBLIT
static void BlitRGBtoRGBPixelAlphaMMX3DNOW(SDL_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	Uint32 *srcp = (Uint32 *)info->s_pixels;
	int srcskip = info->s_skip >> 2;
	Uint32 *dstp = (Uint32 *)info->d_pixels;
	int dstskip = info->d_skip >> 2;
	SDL_PixelFormat* sf = info->src;
	Uint32 chanmask = sf->Rmask | sf->Gmask | sf->Bmask;
	Uint32 amask = sf->Amask;
	Uint32 ashift = sf->Ashift;
	Uint64 multmask;
	
	__m64 src1, dst1, mm_alpha, mm_zero, dmask;

	mm_zero = _mm_setzero_si64(); 
	multmask = ~(0xFFFFi64 << (ashift * 2));
	dmask = *(__m64*) &multmask; 

	while(height--) {
	    DUFFS_LOOP4({
		Uint32 alpha;

		_m_prefetch(srcp + 16);
		_m_prefetch(dstp + 16);

		alpha = *srcp & amask;
		if (alpha == 0) {
			
		} else if (alpha == amask) {
			
			*dstp = (*srcp & chanmask) | (*dstp & ~chanmask);
		} else {
			src1 = _mm_cvtsi32_si64(*srcp); 
			src1 = _mm_unpacklo_pi8(src1, mm_zero); 

			dst1 = _mm_cvtsi32_si64(*dstp); 
			dst1 = _mm_unpacklo_pi8(dst1, mm_zero); 

			mm_alpha = _mm_cvtsi32_si64(alpha); 
			mm_alpha = _mm_srli_si64(mm_alpha, ashift); 
			mm_alpha = _mm_unpacklo_pi16(mm_alpha, mm_alpha); 
			mm_alpha = _mm_unpacklo_pi32(mm_alpha, mm_alpha); 
			mm_alpha = _mm_and_si64(mm_alpha, dmask); 

					    
			src1 = _mm_sub_pi16(src1, dst1);
			src1 = _mm_mullo_pi16(src1, mm_alpha); 
			src1 = _mm_srli_pi16(src1, 8); 
			dst1 = _mm_add_pi8(src1, dst1); 
			dst1 = _mm_packs_pu16(dst1, mm_zero);  
			
			*dstp = _mm_cvtsi64_si32(dst1); 
		}
		++srcp;
		++dstp;
	    }, width);
	    srcp += srcskip;
	    dstp += dstskip;
	}
	_mm_empty();
}

#endif 


#define BLEND16_50(d, s, mask)						\
	((((s & mask) + (d & mask)) >> 1) + (s & d & (~mask & 0xffff)))

#define BLEND2x16_50(d, s, mask)					     \
	(((s & (mask | mask << 16)) >> 1) + ((d & (mask | mask << 16)) >> 1) \
	 + (s & d & (~(mask | mask << 16))))

static void Blit16to16SurfaceAlpha128(SDL_BlitInfo *info, Uint16 mask)
{
	int width = info->d_width;
	int height = info->d_height;
	Uint16 *srcp = (Uint16 *)info->s_pixels;
	int srcskip = info->s_skip >> 1;
	Uint16 *dstp = (Uint16 *)info->d_pixels;
	int dstskip = info->d_skip >> 1;

	while(height--) {
		if(((uintptr_t)srcp ^ (uintptr_t)dstp) & 2) {
			Uint32 prev_sw;
			int w = width;

			
			if((uintptr_t)dstp & 2) {
				Uint16 d = *dstp, s = *srcp;
				*dstp = BLEND16_50(d, s, mask);
				dstp++;
				srcp++;
				w--;
			}
			srcp++;	

			
			prev_sw = ((Uint32 *)srcp)[-1];

			while(w > 1) {
				Uint32 sw, dw, s;
				sw = *(Uint32 *)srcp;
				dw = *(Uint32 *)dstp;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
				s = (prev_sw << 16) + (sw >> 16);
#else
				s = (prev_sw >> 16) + (sw << 16);
#endif
				prev_sw = sw;
				*(Uint32 *)dstp = BLEND2x16_50(dw, s, mask);
				dstp += 2;
				srcp += 2;
				w -= 2;
			}

			
			if(w) {
				Uint16 d = *dstp, s;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
				s = (Uint16)prev_sw;
#else
				s = (Uint16)(prev_sw >> 16);
#endif
				*dstp = BLEND16_50(d, s, mask);
				srcp++;
				dstp++;
			}
			srcp += srcskip - 1;
			dstp += dstskip;
		} else {
			
			int w = width;

			
			if((uintptr_t)srcp & 2) {
				Uint16 d = *dstp, s = *srcp;
				*dstp = BLEND16_50(d, s, mask);
				srcp++;
				dstp++;
				w--;
			}
			

			while(w > 1) {
				Uint32 sw = *(Uint32 *)srcp;
				Uint32 dw = *(Uint32 *)dstp;
				*(Uint32 *)dstp = BLEND2x16_50(dw, sw, mask);
				srcp += 2;
				dstp += 2;
				w -= 2;
			}

			
			if(w) {
				Uint16 d = *dstp, s = *srcp;
				*dstp = BLEND16_50(d, s, mask);
				srcp++;
				dstp++;
			}
			srcp += srcskip;
			dstp += dstskip;
		}
	}
}

#if GCC_ASMBLIT
static void Blit565to565SurfaceAlphaMMX(SDL_BlitInfo *info)
{
	unsigned alpha = info->src->alpha; 
	if(alpha == 128) {
		Blit16to16SurfaceAlpha128(info, 0xf7de);
	} else {
		int width = info->d_width;
		int height = info->d_height;
		Uint16 *srcp = (Uint16 *)info->s_pixels;
		int srcskip = info->s_skip >> 1;
		Uint16 *dstp = (Uint16 *)info->d_pixels;
		int dstskip = info->d_skip >> 1;
		Uint32 s, d;
		Uint64 load;
	  
		alpha &= ~(1+2+4);		
		load = alpha;
		alpha >>= 3;		

		movq_m2r(load, mm0); 
		punpcklwd_r2r(mm0, mm0); 
		punpcklwd_r2r(mm0, mm0); 
		psllq_i2r(3, mm0);
	  
		
		load = 0x07E007E007E007E0ULL;
		movq_m2r(load, mm4); 
		load = 0x001F001F001F001FULL;
		movq_m2r(load, mm7); 
		while(height--) {
			DUFFS_LOOP_QUATRO2(
			{
				s = *srcp++;
				d = *dstp;
				s = (s | s << 16) & 0x07e0f81f;
				d = (d | d << 16) & 0x07e0f81f;
				d += (s - d) * alpha >> 5;
				d &= 0x07e0f81f;
				*dstp++ = d | d >> 16;
			},{
				s = *srcp++;
				d = *dstp;
				s = (s | s << 16) & 0x07e0f81f;
				d = (d | d << 16) & 0x07e0f81f;
				d += (s - d) * alpha >> 5;
				d &= 0x07e0f81f;
				*dstp++ = d | d >> 16;
				s = *srcp++;
				d = *dstp;
				s = (s | s << 16) & 0x07e0f81f;
				d = (d | d << 16) & 0x07e0f81f;
				d += (s - d) * alpha >> 5;
				d &= 0x07e0f81f;
				*dstp++ = d | d >> 16;
			},{
				movq_m2r((*srcp), mm2);
				movq_m2r((*dstp), mm3);

				movq_r2r(mm2, mm5); 
				movq_r2r(mm3, mm6); 
				psrlw_i2r(11, mm5); 
				psrlw_i2r(11, mm6); 

				
				psubw_r2r(mm6, mm5);
				pmullw_r2r(mm0, mm5); 
				psrlw_i2r(11, mm5); 
				paddw_r2r(mm5, mm6); 
				psllw_i2r(11, mm6); 

				movq_r2r(mm6, mm1); 

				
				movq_r2r(mm2, mm5); 
				movq_r2r(mm3, mm6); 
				pand_r2r(mm4, mm5); 
				pand_r2r(mm4, mm6); 

				
				psubw_r2r(mm6, mm5);
				pmulhw_r2r(mm0, mm5); 
				psllw_i2r(5, mm5); 
				paddw_r2r(mm5, mm6); 

				por_r2r(mm6, mm1); 

				
				movq_r2r(mm2, mm5); 
				movq_r2r(mm3, mm6); 
				pand_r2r(mm7, mm5); 
				pand_r2r(mm7, mm6); 

				
				psubw_r2r(mm6, mm5);
				pmullw_r2r(mm0, mm5); 
				psrlw_i2r(11, mm5); 
				paddw_r2r(mm5, mm6); 
				pand_r2r(mm7, mm6); 

				por_r2r(mm6, mm1); 

				movq_r2m(mm1, *dstp); 

				srcp += 4;
				dstp += 4;
			}, width);			
			srcp += srcskip;
			dstp += dstskip;
		}
		emms();
	}
}

static void Blit555to555SurfaceAlphaMMX(SDL_BlitInfo *info)
{
	unsigned alpha = info->src->alpha; 
	if(alpha == 128) {
		Blit16to16SurfaceAlpha128(info, 0xfbde);
	} else {
		int width = info->d_width;
		int height = info->d_height;
		Uint16 *srcp = (Uint16 *)info->s_pixels;
		int srcskip = info->s_skip >> 1;
		Uint16 *dstp = (Uint16 *)info->d_pixels;
		int dstskip = info->d_skip >> 1;
		Uint32 s, d;
		Uint64 load;
	  
		alpha &= ~(1+2+4);		
		load = alpha;
		alpha >>= 3;		

		movq_m2r(load, mm0); 
		punpcklwd_r2r(mm0, mm0); 
		punpcklwd_r2r(mm0, mm0); 
		psllq_i2r(3, mm0);

		
		load = 0x03E003E003E003E0ULL;
		movq_m2r(load, mm4); 
		load = 0x001F001F001F001FULL;
		movq_m2r(load, mm7); 
		while(height--) {
			DUFFS_LOOP_QUATRO2(
			{
				s = *srcp++;
				d = *dstp;
				s = (s | s << 16) & 0x03e07c1f;
				d = (d | d << 16) & 0x03e07c1f;
				d += (s - d) * alpha >> 5;
				d &= 0x03e07c1f;
				*dstp++ = d | d >> 16;
			},{
				s = *srcp++;
				d = *dstp;
				s = (s | s << 16) & 0x03e07c1f;
				d = (d | d << 16) & 0x03e07c1f;
				d += (s - d) * alpha >> 5;
				d &= 0x03e07c1f;
				*dstp++ = d | d >> 16;
			        s = *srcp++;
				d = *dstp;
				s = (s | s << 16) & 0x03e07c1f;
				d = (d | d << 16) & 0x03e07c1f;
				d += (s - d) * alpha >> 5;
				d &= 0x03e07c1f;
				*dstp++ = d | d >> 16;
			},{
				movq_m2r((*srcp), mm2);
				movq_m2r((*dstp), mm3);

				
				psllq_i2r(5, mm4); 

				movq_r2r(mm2, mm5); 
				movq_r2r(mm3, mm6); 
				pand_r2r(mm4, mm5); 
				pand_r2r(mm4, mm6); 

				
				psubw_r2r(mm6, mm5);
				pmulhw_r2r(mm0, mm5); 
				psllw_i2r(5, mm5); 
				paddw_r2r(mm5, mm6); 
				pand_r2r(mm4, mm6); 

				psrlq_i2r(5, mm4); 

				movq_r2r(mm6, mm1); 

				
				movq_r2r(mm2, mm5); 
				movq_r2r(mm3, mm6); 
				pand_r2r(mm4, mm5); 
				pand_r2r(mm4, mm6); 

				
				psubw_r2r(mm6, mm5);
				pmulhw_r2r(mm0, mm5); 
				psllw_i2r(5, mm5); 
				paddw_r2r(mm5, mm6); 

				por_r2r(mm6, mm1); 

				
				movq_r2r(mm2, mm5); 
				movq_r2r(mm3, mm6); 
				pand_r2r(mm7, mm5); 
				pand_r2r(mm7, mm6); 

				
				psubw_r2r(mm6, mm5);
				pmullw_r2r(mm0, mm5); 
				psrlw_i2r(11, mm5); 
				paddw_r2r(mm5, mm6); 
				pand_r2r(mm7, mm6); 

				por_r2r(mm6, mm1); 

				movq_r2m(mm1, *dstp);

				srcp += 4;
				dstp += 4;
			}, width);			
			srcp += srcskip;
			dstp += dstskip;
		}
		emms();
	}
}

#elif MSVC_ASMBLIT
static void Blit565to565SurfaceAlphaMMX(SDL_BlitInfo *info)
{
	unsigned alpha = info->src->alpha;
	if(alpha == 128) {
		Blit16to16SurfaceAlpha128(info, 0xf7de);
	} else {
		int width = info->d_width;
		int height = info->d_height;
		Uint16 *srcp = (Uint16 *)info->s_pixels;
		int srcskip = info->s_skip >> 1;
		Uint16 *dstp = (Uint16 *)info->d_pixels;
		int dstskip = info->d_skip >> 1;
		Uint32 s, d;
	  
		__m64 src1, dst1, src2, dst2, gmask, bmask, mm_res, mm_alpha;

		alpha &= ~(1+2+4);		
		mm_alpha = _mm_set_pi32(0, alpha); 
		alpha >>= 3;		

		mm_alpha = _mm_unpacklo_pi16(mm_alpha, mm_alpha); 
		mm_alpha = _mm_unpacklo_pi32(mm_alpha, mm_alpha); 
		mm_alpha = _mm_slli_si64(mm_alpha, 3);
	  
		
		gmask = _mm_set_pi32(0x07E007E0, 0x07E007E0); 
		bmask = _mm_set_pi32(0x001F001F, 0x001F001F); 
		
		while(height--) {
			DUFFS_LOOP_QUATRO2(
			{
				s = *srcp++;
				d = *dstp;
				s = (s | s << 16) & 0x07e0f81f;
				d = (d | d << 16) & 0x07e0f81f;
				d += (s - d) * alpha >> 5;
				d &= 0x07e0f81f;
				*dstp++ = (Uint16)(d | d >> 16);
			},{
				s = *srcp++;
				d = *dstp;
				s = (s | s << 16) & 0x07e0f81f;
				d = (d | d << 16) & 0x07e0f81f;
				d += (s - d) * alpha >> 5;
				d &= 0x07e0f81f;
				*dstp++ = (Uint16)(d | d >> 16);
				s = *srcp++;
				d = *dstp;
				s = (s | s << 16) & 0x07e0f81f;
				d = (d | d << 16) & 0x07e0f81f;
				d += (s - d) * alpha >> 5;
				d &= 0x07e0f81f;
				*dstp++ = (Uint16)(d | d >> 16);
			},{
				src1 = *(__m64*)srcp; 
				dst1 = *(__m64*)dstp; 

				
				src2 = src1;
				src2 = _mm_srli_pi16(src2, 11); 

				dst2 = dst1;
				dst2 = _mm_srli_pi16(dst2, 11); 

				
				src2 = _mm_sub_pi16(src2, dst2);
				src2 = _mm_mullo_pi16(src2, mm_alpha); 
				src2 = _mm_srli_pi16(src2, 11); 
				dst2 = _mm_add_pi16(src2, dst2); 
				dst2 = _mm_slli_pi16(dst2, 11); 

				mm_res = dst2; 

				
				src2 = src1;
				src2 = _mm_and_si64(src2, gmask); 

				dst2 = dst1;
				dst2 = _mm_and_si64(dst2, gmask); 

				
				src2 = _mm_sub_pi16(src2, dst2);
				src2 = _mm_mulhi_pi16(src2, mm_alpha); 
				src2 = _mm_slli_pi16(src2, 5); 
				dst2 = _mm_add_pi16(src2, dst2); 

				mm_res = _mm_or_si64(mm_res, dst2); 

				
				src2 = src1;
				src2 = _mm_and_si64(src2, bmask); 

				dst2 = dst1;
				dst2 = _mm_and_si64(dst2, bmask); 

				
				src2 = _mm_sub_pi16(src2, dst2);
				src2 = _mm_mullo_pi16(src2, mm_alpha); 
				src2 = _mm_srli_pi16(src2, 11); 
				dst2 = _mm_add_pi16(src2, dst2); 
				dst2 = _mm_and_si64(dst2, bmask); 

				mm_res = _mm_or_si64(mm_res, dst2); 

				*(__m64*)dstp = mm_res; 

				srcp += 4;
				dstp += 4;
			}, width);			
			srcp += srcskip;
			dstp += dstskip;
		}
		_mm_empty();
	}
}

static void Blit555to555SurfaceAlphaMMX(SDL_BlitInfo *info)
{
	unsigned alpha = info->src->alpha;
	if(alpha == 128) {
		Blit16to16SurfaceAlpha128(info, 0xfbde);
	} else {
		int width = info->d_width;
		int height = info->d_height;
		Uint16 *srcp = (Uint16 *)info->s_pixels;
		int srcskip = info->s_skip >> 1;
		Uint16 *dstp = (Uint16 *)info->d_pixels;
		int dstskip = info->d_skip >> 1;
		Uint32 s, d;
	  
		__m64 src1, dst1, src2, dst2, rmask, gmask, bmask, mm_res, mm_alpha;

		alpha &= ~(1+2+4);		
		mm_alpha = _mm_set_pi32(0, alpha); 
		alpha >>= 3;		

		mm_alpha = _mm_unpacklo_pi16(mm_alpha, mm_alpha); 
		mm_alpha = _mm_unpacklo_pi32(mm_alpha, mm_alpha); 
		mm_alpha = _mm_slli_si64(mm_alpha, 3);
	  
		
		rmask = _mm_set_pi32(0x7C007C00, 0x7C007C00); 
		gmask = _mm_set_pi32(0x03E003E0, 0x03E003E0); 
		bmask = _mm_set_pi32(0x001F001F, 0x001F001F); 

		while(height--) {
			DUFFS_LOOP_QUATRO2(
			{
				s = *srcp++;
				d = *dstp;
				s = (s | s << 16) & 0x03e07c1f;
				d = (d | d << 16) & 0x03e07c1f;
				d += (s - d) * alpha >> 5;
				d &= 0x03e07c1f;
				*dstp++ = (Uint16)(d | d >> 16);
			},{
				s = *srcp++;
				d = *dstp;
				s = (s | s << 16) & 0x03e07c1f;
				d = (d | d << 16) & 0x03e07c1f;
				d += (s - d) * alpha >> 5;
				d &= 0x03e07c1f;
				*dstp++ = (Uint16)(d | d >> 16);
			        s = *srcp++;
				d = *dstp;
				s = (s | s << 16) & 0x03e07c1f;
				d = (d | d << 16) & 0x03e07c1f;
				d += (s - d) * alpha >> 5;
				d &= 0x03e07c1f;
				*dstp++ = (Uint16)(d | d >> 16);
			},{
				src1 = *(__m64*)srcp; 
				dst1 = *(__m64*)dstp; 

				
				src2 = src1;
				src2 = _mm_and_si64(src2, rmask); 

				dst2 = dst1;
				dst2 = _mm_and_si64(dst2, rmask); 

				
				src2 = _mm_sub_pi16(src2, dst2);
				src2 = _mm_mulhi_pi16(src2, mm_alpha); 
				src2 = _mm_slli_pi16(src2, 5); 
				dst2 = _mm_add_pi16(src2, dst2); 
				dst2 = _mm_and_si64(dst2, rmask); 

				mm_res = dst2; 
				
				
				src2 = src1;
				src2 = _mm_and_si64(src2, gmask); 

				dst2 = dst1;
				dst2 = _mm_and_si64(dst2, gmask); 

				
				src2 = _mm_sub_pi16(src2, dst2);
				src2 = _mm_mulhi_pi16(src2, mm_alpha); 
				src2 = _mm_slli_pi16(src2, 5); 
				dst2 = _mm_add_pi16(src2, dst2); 

				mm_res = _mm_or_si64(mm_res, dst2); 

				
				src2 = src1; 
				src2 = _mm_and_si64(src2, bmask); 

				dst2 = dst1; 
				dst2 = _mm_and_si64(dst2, bmask); 

				
				src2 = _mm_sub_pi16(src2, dst2);
				src2 = _mm_mullo_pi16(src2, mm_alpha); 
				src2 = _mm_srli_pi16(src2, 11); 
				dst2 = _mm_add_pi16(src2, dst2); 
				dst2 = _mm_and_si64(dst2, bmask); 

				mm_res = _mm_or_si64(mm_res, dst2); 

				*(__m64*)dstp = mm_res; 

				srcp += 4;
				dstp += 4;
			}, width);			
			srcp += srcskip;
			dstp += dstskip;
		}
		_mm_empty();
	}
}
#endif 

static void Blit565to565SurfaceAlpha(SDL_BlitInfo *info)
{
	unsigned alpha = info->src->alpha;
	if(alpha == 128) {
		Blit16to16SurfaceAlpha128(info, 0xf7de);
	} else {
		int width = info->d_width;
		int height = info->d_height;
		Uint16 *srcp = (Uint16 *)info->s_pixels;
		int srcskip = info->s_skip >> 1;
		Uint16 *dstp = (Uint16 *)info->d_pixels;
		int dstskip = info->d_skip >> 1;
		alpha >>= 3;	

		while(height--) {
			DUFFS_LOOP4({
				Uint32 s = *srcp++;
				Uint32 d = *dstp;
				s = (s | s << 16) & 0x07e0f81f;
				d = (d | d << 16) & 0x07e0f81f;
				d += (s - d) * alpha >> 5;
				d &= 0x07e0f81f;
				*dstp++ = (Uint16)(d | d >> 16);
			}, width);
			srcp += srcskip;
			dstp += dstskip;
		}
	}
}

static void Blit555to555SurfaceAlpha(SDL_BlitInfo *info)
{
	unsigned alpha = info->src->alpha; 
	if(alpha == 128) {
		Blit16to16SurfaceAlpha128(info, 0xfbde);
	} else {
		int width = info->d_width;
		int height = info->d_height;
		Uint16 *srcp = (Uint16 *)info->s_pixels;
		int srcskip = info->s_skip >> 1;
		Uint16 *dstp = (Uint16 *)info->d_pixels;
		int dstskip = info->d_skip >> 1;
		alpha >>= 3;		

		while(height--) {
			DUFFS_LOOP4({
				Uint32 s = *srcp++;
				Uint32 d = *dstp;
				s = (s | s << 16) & 0x03e07c1f;
				d = (d | d << 16) & 0x03e07c1f;
				d += (s - d) * alpha >> 5;
				d &= 0x03e07c1f;
				*dstp++ = (Uint16)(d | d >> 16);
			}, width);
			srcp += srcskip;
			dstp += dstskip;
		}
	}
}

static void BlitARGBto565PixelAlpha(SDL_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	Uint32 *srcp = (Uint32 *)info->s_pixels;
	int srcskip = info->s_skip >> 2;
	Uint16 *dstp = (Uint16 *)info->d_pixels;
	int dstskip = info->d_skip >> 1;

	while(height--) {
	    DUFFS_LOOP4({
		Uint32 s = *srcp;
		unsigned alpha = s >> 27; 
		if(alpha) {   
		  if(alpha == (SDL_ALPHA_OPAQUE >> 3)) {
		    *dstp = (Uint16)((s >> 8 & 0xf800) + (s >> 5 & 0x7e0) + (s >> 3  & 0x1f));
		  } else {
		    Uint32 d = *dstp;
		    s = ((s & 0xfc00) << 11) + (s >> 8 & 0xf800)
		      + (s >> 3 & 0x1f);
		    d = (d | d << 16) & 0x07e0f81f;
		    d += (s - d) * alpha >> 5;
		    d &= 0x07e0f81f;
		    *dstp = (Uint16)(d | d >> 16);
		  }
		}
		srcp++;
		dstp++;
	    }, width);
	    srcp += srcskip;
	    dstp += dstskip;
	}
}

static void BlitARGBto555PixelAlpha(SDL_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	Uint32 *srcp = (Uint32 *)info->s_pixels;
	int srcskip = info->s_skip >> 2;
	Uint16 *dstp = (Uint16 *)info->d_pixels;
	int dstskip = info->d_skip >> 1;

	while(height--) {
	    DUFFS_LOOP4({
		unsigned alpha;
		Uint32 s = *srcp;
		alpha = s >> 27; 
		if(alpha) {   
		  if(alpha == (SDL_ALPHA_OPAQUE >> 3)) {
		    *dstp = (Uint16)((s >> 9 & 0x7c00) + (s >> 6 & 0x3e0) + (s >> 3  & 0x1f));
		  } else {
		    Uint32 d = *dstp;
		    s = ((s & 0xf800) << 10) + (s >> 9 & 0x7c00)
		      + (s >> 3 & 0x1f);
		    d = (d | d << 16) & 0x03e07c1f;
		    d += (s - d) * alpha >> 5;
		    d &= 0x03e07c1f;
		    *dstp = (Uint16)(d | d >> 16);
		  }
		}
		srcp++;
		dstp++;
	    }, width);
	    srcp += srcskip;
	    dstp += dstskip;
	}
}

static void BlitNtoNSurfaceAlpha(SDL_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	Uint8 *src = info->s_pixels;
	int srcskip = info->s_skip;
	Uint8 *dst = info->d_pixels;
	int dstskip = info->d_skip;
	SDL_PixelFormat *srcfmt = info->src;
	SDL_PixelFormat *dstfmt = info->dst;
	int srcbpp = srcfmt->BytesPerPixel;
	int dstbpp = dstfmt->BytesPerPixel;
	unsigned sA = srcfmt->alpha;
	unsigned dA = dstfmt->Amask ? SDL_ALPHA_OPAQUE : 0;

	if(sA) {
	  while ( height-- ) {
	    DUFFS_LOOP4(
	    {
		Uint32 Pixel;
		unsigned sR;
		unsigned sG;
		unsigned sB;
		unsigned dR;
		unsigned dG;
		unsigned dB;
		DISEMBLE_RGB(src, srcbpp, srcfmt, Pixel, sR, sG, sB);
		DISEMBLE_RGB(dst, dstbpp, dstfmt, Pixel, dR, dG, dB);
		ALPHA_BLEND(sR, sG, sB, sA, dR, dG, dB);
		ASSEMBLE_RGBA(dst, dstbpp, dstfmt, dR, dG, dB, dA);
		src += srcbpp;
		dst += dstbpp;
	    },
	    width);
	    src += srcskip;
	    dst += dstskip;
	  }
	}
}

static void BlitNtoNSurfaceAlphaKey(SDL_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	Uint8 *src = info->s_pixels;
	int srcskip = info->s_skip;
	Uint8 *dst = info->d_pixels;
	int dstskip = info->d_skip;
	SDL_PixelFormat *srcfmt = info->src;
	SDL_PixelFormat *dstfmt = info->dst;
	Uint32 ckey = srcfmt->colorkey;
	int srcbpp = srcfmt->BytesPerPixel;
	int dstbpp = dstfmt->BytesPerPixel;
	unsigned sA = srcfmt->alpha;
	unsigned dA = dstfmt->Amask ? SDL_ALPHA_OPAQUE : 0;

	while ( height-- ) {
	    DUFFS_LOOP4(
	    {
		Uint32 Pixel;
		unsigned sR;
		unsigned sG;
		unsigned sB;
		unsigned dR;
		unsigned dG;
		unsigned dB;
		RETRIEVE_RGB_PIXEL(src, srcbpp, Pixel);
		if(sA && Pixel != ckey) {
		    RGB_FROM_PIXEL(Pixel, srcfmt, sR, sG, sB);
		    DISEMBLE_RGB(dst, dstbpp, dstfmt, Pixel, dR, dG, dB);
		    ALPHA_BLEND(sR, sG, sB, sA, dR, dG, dB);
		    ASSEMBLE_RGBA(dst, dstbpp, dstfmt, dR, dG, dB, dA);
		}
		src += srcbpp;
		dst += dstbpp;
	    },
	    width);
	    src += srcskip;
	    dst += dstskip;
	}
}

static void BlitNtoNPixelAlpha(SDL_BlitInfo *info)
{
	int width = info->d_width;
	int height = info->d_height;
	Uint8 *src = info->s_pixels;
	int srcskip = info->s_skip;
	Uint8 *dst = info->d_pixels;
	int dstskip = info->d_skip;
	SDL_PixelFormat *srcfmt = info->src;
	SDL_PixelFormat *dstfmt = info->dst;

	int  srcbpp;
	int  dstbpp;

	
	srcbpp = srcfmt->BytesPerPixel;
	dstbpp = dstfmt->BytesPerPixel;

	while ( height-- ) {
	    DUFFS_LOOP4(
	    {
		Uint32 Pixel;
		unsigned sR;
		unsigned sG;
		unsigned sB;
		unsigned dR;
		unsigned dG;
		unsigned dB;
		unsigned sA;
		unsigned dA;
		DISEMBLE_RGBA(src, srcbpp, srcfmt, Pixel, sR, sG, sB, sA);
		if(sA) {
		  DISEMBLE_RGBA(dst, dstbpp, dstfmt, Pixel, dR, dG, dB, dA);
		  ALPHA_BLEND(sR, sG, sB, sA, dR, dG, dB);
		  ASSEMBLE_RGBA(dst, dstbpp, dstfmt, dR, dG, dB, dA);
		}
		src += srcbpp;
		dst += dstbpp;
	    },
	    width);
	    src += srcskip;
	    dst += dstskip;
	}
}


SDL_loblit SDL_CalculateAlphaBlit(SDL_Surface *surface, int blit_index)
{
    SDL_PixelFormat *sf = surface->format;
    SDL_PixelFormat *df = surface->map->dst->format;

    if(sf->Amask == 0) {
	if((surface->flags & SDL_SRCCOLORKEY) == SDL_SRCCOLORKEY) {
	    if(df->BytesPerPixel == 1)
		return BlitNto1SurfaceAlphaKey;
	    else
#if SDL_ALTIVEC_BLITTERS
	if (sf->BytesPerPixel == 4 && df->BytesPerPixel == 4 &&
	    !(surface->map->dst->flags & SDL_HWSURFACE) && SDL_HasAltiVec())
            return Blit32to32SurfaceAlphaKeyAltivec;
        else
#endif
            return BlitNtoNSurfaceAlphaKey;
	} else {
	    
	    switch(df->BytesPerPixel) {
	    case 1:
		return BlitNto1SurfaceAlpha;

	    case 2:
		if(surface->map->identity) {
		    if(df->Gmask == 0x7e0)
		    {
#if MMX_ASMBLIT
		if(SDL_HasMMX())
			return Blit565to565SurfaceAlphaMMX;
		else
#endif
			return Blit565to565SurfaceAlpha;
		    }
		    else if(df->Gmask == 0x3e0)
		    {
#if MMX_ASMBLIT
		if(SDL_HasMMX())
			return Blit555to555SurfaceAlphaMMX;
		else
#endif
			return Blit555to555SurfaceAlpha;
		    }
		}
		return BlitNtoNSurfaceAlpha;

	    case 4:
		if(sf->Rmask == df->Rmask
		   && sf->Gmask == df->Gmask
		   && sf->Bmask == df->Bmask
		   && sf->BytesPerPixel == 4)
		{
#if MMX_ASMBLIT
			if(sf->Rshift % 8 == 0
			   && sf->Gshift % 8 == 0
			   && sf->Bshift % 8 == 0
			   && SDL_HasMMX())
			    return BlitRGBtoRGBSurfaceAlphaMMX;
#endif
			if((sf->Rmask | sf->Gmask | sf->Bmask) == 0xffffff)
			{
#if SDL_ALTIVEC_BLITTERS
				if(!(surface->map->dst->flags & SDL_HWSURFACE)
					&& SDL_HasAltiVec())
					return BlitRGBtoRGBSurfaceAlphaAltivec;
#endif
				return BlitRGBtoRGBSurfaceAlpha;
			}
		}
#if SDL_ALTIVEC_BLITTERS
		if((sf->BytesPerPixel == 4) &&
		   !(surface->map->dst->flags & SDL_HWSURFACE) && SDL_HasAltiVec())
			return Blit32to32SurfaceAlphaAltivec;
		else
#endif
			return BlitNtoNSurfaceAlpha;

	    case 3:
	    default:
		return BlitNtoNSurfaceAlpha;
	    }
	}
    } else {
	
	switch(df->BytesPerPixel) {
	case 1:
	    return BlitNto1PixelAlpha;

	case 2:
#if SDL_ALTIVEC_BLITTERS
	if(sf->BytesPerPixel == 4 && !(surface->map->dst->flags & SDL_HWSURFACE) &&
           df->Gmask == 0x7e0 &&
	   df->Bmask == 0x1f && SDL_HasAltiVec())
            return Blit32to565PixelAlphaAltivec;
        else
#endif
	    if(sf->BytesPerPixel == 4 && sf->Amask == 0xff000000
	       && sf->Gmask == 0xff00
	       && ((sf->Rmask == 0xff && df->Rmask == 0x1f)
		   || (sf->Bmask == 0xff && df->Bmask == 0x1f))) {
		if(df->Gmask == 0x7e0)
		    return BlitARGBto565PixelAlpha;
		else if(df->Gmask == 0x3e0)
		    return BlitARGBto555PixelAlpha;
	    }
	    return BlitNtoNPixelAlpha;

	case 4:
	    if(sf->Rmask == df->Rmask
	       && sf->Gmask == df->Gmask
	       && sf->Bmask == df->Bmask
	       && sf->BytesPerPixel == 4)
	    {
#if MMX_ASMBLIT
		if(sf->Rshift % 8 == 0
		   && sf->Gshift % 8 == 0
		   && sf->Bshift % 8 == 0
		   && sf->Ashift % 8 == 0
		   && sf->Aloss == 0)
		{
			if(SDL_Has3DNow())
				return BlitRGBtoRGBPixelAlphaMMX3DNOW;
			if(SDL_HasMMX())
				return BlitRGBtoRGBPixelAlphaMMX;
		}
#endif
		if(sf->Amask == 0xff000000)
		{
#if SDL_ALTIVEC_BLITTERS
			if(!(surface->map->dst->flags & SDL_HWSURFACE)
				&& SDL_HasAltiVec())
				return BlitRGBtoRGBPixelAlphaAltivec;
#endif
			return BlitRGBtoRGBPixelAlpha;
		}
	    }
#if SDL_ALTIVEC_BLITTERS
	    if (sf->Amask && sf->BytesPerPixel == 4 &&
	        !(surface->map->dst->flags & SDL_HWSURFACE) && SDL_HasAltiVec())
		return Blit32to32PixelAlphaAltivec;
	    else
#endif
		return BlitNtoNPixelAlpha;

	case 3:
	default:
	    return BlitNtoNPixelAlpha;
	}
    }
}

