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

#if (__GNUC__ > 2) && defined(__i386__) && __OPTIMIZE__ && SDL_ASSEMBLY_ROUTINES

#include "SDL_stdinc.h"

#include "mmx.h"


static mmx_t MMX_0080w    = { .ud = {0x00800080, 0x00800080} };
static mmx_t MMX_00FFw    = { .ud = {0x00ff00ff, 0x00ff00ff} };
static mmx_t MMX_FF00w    = { .ud = {0xff00ff00, 0xff00ff00} };

static mmx_t MMX_Ycoeff   = { .uw = {0x004a, 0x004a, 0x004a, 0x004a} };

static mmx_t MMX_UbluRGB  = { .uw = {0x0072, 0x0072, 0x0072, 0x0072} };
static mmx_t MMX_VredRGB  = { .uw = {0x0059, 0x0059, 0x0059, 0x0059} };
static mmx_t MMX_UgrnRGB  = { .uw = {0xffea, 0xffea, 0xffea, 0xffea} };
static mmx_t MMX_VgrnRGB  = { .uw = {0xffd2, 0xffd2, 0xffd2, 0xffd2} };

static mmx_t MMX_Ublu5x5  = { .uw = {0x0081, 0x0081, 0x0081, 0x0081} };
static mmx_t MMX_Vred5x5  = { .uw = {0x0066, 0x0066, 0x0066, 0x0066} };
static mmx_t MMX_Ugrn565  = { .uw = {0xffe8, 0xffe8, 0xffe8, 0xffe8} };
static mmx_t MMX_Vgrn565  = { .uw = {0xffcd, 0xffcd, 0xffcd, 0xffcd} };

static mmx_t MMX_red565   = { .uw = {0xf800, 0xf800, 0xf800, 0xf800} };
static mmx_t MMX_grn565   = { .uw = {0x07e0, 0x07e0, 0x07e0, 0x07e0} };


void ColorRGBDitherYV12MMX1X( int *colortab, Uint32 *rgb_2_pix,
                              unsigned char *lum, unsigned char *cr,
                              unsigned char *cb, unsigned char *out,
                              int rows, int cols, int mod )
{
	Uint32 *row1;
	Uint32 *row2;

	unsigned char* y = lum +cols*rows;    
	int x = 0;
	row1 = (Uint32 *)out;                 
	row2 = (Uint32 *)out+cols+mod;        
	mod = (mod+cols+mod)*4;               

	__asm__ __volatile__ (
		
		
		"pushl $0\n"  
		"pushl %%ebx\n"  
		"movl %0, %%ebx\n"  
		"movl %%ebx,4(%%esp)\n"  
		"popl %%ebx\n"  

		".align 8\n"
		"1:\n"

		
		"pushl %%ebx\n"
		"movl 4(%%esp),%%ebx\n"
		"movd (%%ebx),%%mm1\n"   
		"popl %%ebx\n"
		"pxor %%mm7,%%mm7\n"      
		"movd (%2), %%mm2\n"           
		"punpcklbw %%mm7,%%mm1\n" 
		"punpckldq %%mm1,%%mm1\n" 
		"psubw %9,%%mm1\n"        

		 
		"movq %%mm1,%%mm0\n"           
		"pmullw %10,%%mm0\n"           
		"pmullw %11,%%mm1\n"           
		"psraw  $6, %%mm0\n"           
		"psraw  $6, %%mm1\n"           

		
		
		"movq (%2,%4),%%mm3\n"         
		"punpckldq %%mm3,%%mm2\n"      
		"movq %%mm2,%%mm4\n"           
		"pand %12,%%mm2\n"             
		"pand %13,%%mm4\n"             
		"psrlw $8,%%mm2\n"             

		
		"movq %%mm2,%%mm5\n"           
		"movq %%mm4,%%mm6\n"           
		"paddsw  %%mm1, %%mm5\n"       
		"paddsw  %%mm1, %%mm6\n"       
		"packuswb %%mm5,%%mm5\n"       
		"packuswb %%mm6,%%mm6\n"       
		"pxor %%mm7,%%mm7\n"      
		"punpcklbw %%mm5,%%mm6\n"      

		
		"movd (%1), %%mm1\n"      
		"punpcklbw %%mm7,%%mm1\n" 
		"punpckldq %%mm1,%%mm1\n" 
		"psubw %9,%%mm1\n"        
		
		"movq %%mm1,%%mm5\n"            
		"pmullw %14,%%mm5\n"            
		"pmullw %15,%%mm1\n"            
		"psraw  $6, %%mm5\n"            
		"psraw  $6, %%mm1\n"            

		
		"movq %%mm2,%%mm3\n"      
		"movq %%mm4,%%mm7\n"      
		"paddsw  %%mm5, %%mm3\n"  
		"paddsw  %%mm5, %%mm7\n"  
		"paddsw  %%mm0, %%mm3\n"  
		"paddsw  %%mm0, %%mm7\n"  
		"packuswb %%mm3,%%mm3\n"  
		"packuswb %%mm7,%%mm7\n"  
		"punpcklbw %%mm3,%%mm7\n" 

		
		"movq %%mm2,%%mm3\n"         
		"movq %%mm4,%%mm5\n"         
		"paddsw  %%mm1, %%mm3\n"     
		"paddsw  %%mm1, %%mm5\n"     
		"packuswb %%mm3,%%mm3\n"     
		"packuswb %%mm5,%%mm5\n"     
		"punpcklbw %%mm3,%%mm5\n"    

		

		"pxor %%mm2,%%mm2\n"           
		"pxor %%mm4,%%mm4\n"           
		"movq %%mm6,%%mm1\n"           
		"movq %%mm5,%%mm3\n"           
		
		"punpcklbw %%mm4,%%mm1\n"      
		"punpcklbw %%mm4,%%mm3\n"      
		"movq %%mm1,%%mm2\n"           
		"movq %%mm3,%%mm0\n"           
		"punpcklwd %%mm1,%%mm3\n"      
		"punpckhwd %%mm2,%%mm0\n"      

		"pxor %%mm2,%%mm2\n"           
		"movq %%mm7,%%mm1\n"           
		"punpcklbw %%mm1,%%mm2\n"      
		"punpcklwd %%mm4,%%mm2\n"      
		"por %%mm3, %%mm2\n"          
		"movq %%mm2,(%3)\n"          

		"pxor %%mm2,%%mm2\n"           
		"punpcklbw %%mm1,%%mm4\n"      
		"punpckhwd %%mm2,%%mm4\n"      
		"por %%mm0, %%mm4\n"          
		"movq %%mm4,8(%3)\n"         
		 
		
		
		"pxor %%mm2,%%mm2\n"           
		"punpckhbw %%mm2,%%mm6\n"      
		"punpckhbw %%mm1,%%mm5\n"      
		"movq %%mm5,%%mm1\n"           
		"punpcklwd %%mm6,%%mm1\n"      
		"movq %%mm1,(%5)\n"          
		"punpckhwd %%mm6,%%mm5\n"      
		"movq %%mm5,8(%5)\n"         
		 
		"addl $4,%2\n"            
		"leal 16(%3),%3\n"        
		"leal 16(%5),%5\n"        
		"addl $2,(%%esp)\n"        
		"addl $2,%1\n"           

		"addl $4,%6\n"            
		"cmpl %4,%6\n"

		"jl 1b\n"
		"addl %4,%2\n" 
		"addl %8,%3\n" 
		"addl %8,%5\n" 
		"movl $0,%6\n" 
		"cmpl %7,%2\n"
		"jl 1b\n"

		"addl $4,%%esp\n"  
		"emms\n"  
		:
		: "m" (cr), "r"(cb),"r"(lum),
		  "r"(row1),"r"(cols),"r"(row2),"m"(x),"m"(y),"m"(mod),
		  "m"(MMX_0080w),"m"(MMX_VgrnRGB),"m"(MMX_VredRGB),
		  "m"(MMX_FF00w),"m"(MMX_00FFw),"m"(MMX_UgrnRGB),
		  "m"(MMX_UbluRGB)
	);
}

void Color565DitherYV12MMX1X( int *colortab, Uint32 *rgb_2_pix,
                             unsigned char *lum, unsigned char *cr,
                             unsigned char *cb, unsigned char *out,
                             int rows, int cols, int mod )
{
	Uint16 *row1;
	Uint16 *row2;

	unsigned char* y = lum +cols*rows;    
	int x = 0;
	row1 = (Uint16 *)out;                 
	row2 = (Uint16 *)out+cols+mod;        
	mod = (mod+cols+mod)*2;               

	__asm__ __volatile__(
		
		
		"pushl $0\n"  
		"pushl %%ebx\n"  
		"movl %0, %%ebx\n"  
		"movl %%ebx, 4(%%esp)\n"  
		"popl %%ebx\n"  

		".align 8\n"
		"1:\n"
		"movd           (%1),                   %%mm0\n" 
		"pxor           %%mm7,                  %%mm7\n"
		"pushl %%ebx\n"
		"movl 4(%%esp), %%ebx\n"
		"movd (%%ebx), %%mm1\n"   
		"popl %%ebx\n"

		"punpcklbw      %%mm7,                  %%mm0\n" 
		"punpcklbw      %%mm7,                  %%mm1\n" 
		"psubw          %9,                     %%mm0\n"
		"psubw          %9,                     %%mm1\n"
		"movq           %%mm0,                  %%mm2\n" 
		"movq           %%mm1,                  %%mm3\n" 
		"pmullw         %10,                    %%mm2\n" 
		"movq           (%2),                   %%mm6\n" 
		"pmullw         %11,                    %%mm0\n" 
		"pand           %12,                    %%mm6\n" 
		"pmullw         %13,                    %%mm3\n" 
		"movq           (%2),                   %%mm7\n" 
		"pmullw         %14,                    %%mm1\n" 
		"psrlw          $8,                     %%mm7\n"        
		"pmullw         %15,                    %%mm6\n" 
		"paddw          %%mm3,                  %%mm2\n" 
		"pmullw         %15,                    %%mm7\n" 

		"movq           %%mm6,                  %%mm4\n" 
		"paddw          %%mm0,                  %%mm6\n" 
		"movq           %%mm4,                  %%mm5\n" 
		"paddw          %%mm1,                  %%mm4\n" 
		"paddw          %%mm2,                  %%mm5\n" 
		"psraw          $6,                     %%mm4\n" 
		"movq           %%mm7,                  %%mm3\n" 
		"psraw          $6,                     %%mm5\n" 
		"paddw          %%mm0,                  %%mm7\n" 
		"psraw          $6,                     %%mm6\n" 
		"packuswb       %%mm4,                  %%mm4\n" 
		"packuswb       %%mm5,                  %%mm5\n" 
		"packuswb       %%mm6,                  %%mm6\n" 
		"punpcklbw      %%mm4,                  %%mm4\n"
		"punpcklbw      %%mm5,                  %%mm5\n"

		"pand           %16,                    %%mm4\n"
		"psllw          $3,                     %%mm5\n" 
		"punpcklbw      %%mm6,                  %%mm6\n"
		"pand           %17,                    %%mm5\n"
		"pand           %16,                    %%mm6\n"
		"por            %%mm5,                  %%mm4\n" 
		"psrlw          $11,                    %%mm6\n" 
		"movq           %%mm3,                  %%mm5\n" 
		"paddw          %%mm1,                  %%mm3\n" 
		"paddw          %%mm2,                  %%mm5\n" 
		"psraw          $6,                     %%mm3\n" 
		"por            %%mm6,                  %%mm4\n" 
		"psraw          $6,                     %%mm5\n" 
		"movq           (%2, %4),               %%mm6\n" 
		"psraw          $6,                     %%mm7\n"
		"packuswb       %%mm3,                  %%mm3\n"
		"packuswb       %%mm5,                  %%mm5\n"
		"packuswb       %%mm7,                  %%mm7\n"
		"pand           %12,                    %%mm6\n" 
		"punpcklbw      %%mm3,                  %%mm3\n"
		"punpcklbw      %%mm5,                  %%mm5\n"
		"pmullw         %15,                    %%mm6\n" 
		"punpcklbw      %%mm7,                  %%mm7\n"
		"psllw          $3,                     %%mm5\n" 
		"pand           %16,                    %%mm7\n"
		"pand           %16,                    %%mm3\n"
		"psrlw          $11,                    %%mm7\n" 
		"pand           %17,                    %%mm5\n"
		"por            %%mm7,                  %%mm3\n"
		"movq           (%2,%4),                %%mm7\n" 
		"por            %%mm5,                  %%mm3\n" 
		"psrlw          $8,                     %%mm7\n" 
		"movq           %%mm4,                  %%mm5\n"
		"punpcklwd      %%mm3,                  %%mm4\n"
		"pmullw         %15,                    %%mm7\n" 
		"punpckhwd      %%mm3,                  %%mm5\n"

		"movq           %%mm4,                  (%3)\n"  
		"movq           %%mm5,                  8(%3)\n" 

		"movq           %%mm6,                  %%mm4\n" 
		"paddw          %%mm0,                  %%mm6\n" 

		"movq           %%mm4,                  %%mm5\n" 
		"paddw          %%mm1,                  %%mm4\n" 
		"paddw          %%mm2,                  %%mm5\n" 
		"psraw          $6,                     %%mm4\n"
		"movq           %%mm7,                  %%mm3\n" 
		"psraw          $6,                     %%mm5\n"
		"paddw          %%mm0,                  %%mm7\n" 
		"psraw          $6,                     %%mm6\n" 
		"movq           %%mm3,                  %%mm0\n" 
		"packuswb       %%mm4,                  %%mm4\n"
		"paddw          %%mm1,                  %%mm3\n" 
		"packuswb       %%mm5,                  %%mm5\n"
		"paddw          %%mm2,                  %%mm0\n" 
		"packuswb       %%mm6,                  %%mm6\n"
		"punpcklbw      %%mm4,                  %%mm4\n"
		"punpcklbw      %%mm5,                  %%mm5\n"
		"punpcklbw      %%mm6,                  %%mm6\n"
		"psllw          $3,                     %%mm5\n" 
		"pand           %16,                    %%mm4\n"
		"psraw          $6,                     %%mm3\n" 
		"psraw          $6,                     %%mm0\n"
		"pand           %16,                    %%mm6\n" 
		"pand           %17,                    %%mm5\n"
		"psrlw          $11,                    %%mm6\n" 
		"por            %%mm5,                  %%mm4\n"
		"psraw          $6,                     %%mm7\n"
		"por            %%mm6,                  %%mm4\n"
		"packuswb       %%mm3,                  %%mm3\n"
		"packuswb       %%mm0,                  %%mm0\n"
		"packuswb       %%mm7,                  %%mm7\n"
		"punpcklbw      %%mm3,                  %%mm3\n"
		"punpcklbw      %%mm0,                  %%mm0\n"
		"punpcklbw      %%mm7,                  %%mm7\n"
		"pand           %16,                    %%mm3\n"
		"pand           %16,                    %%mm7\n" 
		"psllw          $3,                     %%mm0\n" 
		"psrlw          $11,                    %%mm7\n"
		"pand           %17,                    %%mm0\n"
		"por            %%mm7,                  %%mm3\n"
		"por            %%mm0,                  %%mm3\n"

		"movq           %%mm4,                  %%mm5\n"

		"punpcklwd      %%mm3,                  %%mm4\n"
		"punpckhwd      %%mm3,                  %%mm5\n"

		"movq           %%mm4,                  (%5)\n"
		"movq           %%mm5,                  8(%5)\n"

		"addl           $8,                     %6\n"
		"addl           $8,                     %2\n"
		"addl           $4,                     (%%esp)\n"
		"addl           $4,                     %1\n"
		"cmpl           %4,                     %6\n"
		"leal           16(%3),                 %3\n"
		"leal           16(%5),%5\n" 

		"jl             1b\n"
		"addl           %4,     %2\n" 
		"addl           %8,     %3\n" 
		"addl           %8,     %5\n" 
		"movl           $0,     %6\n" 
		"cmpl           %7,     %2\n"
		"jl             1b\n"
		"addl $4, %%esp\n"  
		"emms\n"
		:
		: "m" (cr), "r"(cb),"r"(lum),
		  "r"(row1),"r"(cols),"r"(row2),"m"(x),"m"(y),"m"(mod),
		  "m"(MMX_0080w),"m"(MMX_Ugrn565),"m"(MMX_Ublu5x5),
		  "m"(MMX_00FFw),"m"(MMX_Vgrn565),"m"(MMX_Vred5x5),
		  "m"(MMX_Ycoeff),"m"(MMX_red565),"m"(MMX_grn565)
	);
}


#endif 

