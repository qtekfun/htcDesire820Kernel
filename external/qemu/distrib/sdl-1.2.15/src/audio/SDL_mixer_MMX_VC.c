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

#include "SDL_mixer_MMX_VC.h"

#if defined(SDL_BUGGY_MMX_MIXERS) 
#if ((defined(_MSC_VER) && defined(_M_IX86)) || defined(__WATCOMC__)) && defined(SDL_ASSEMBLY_ROUTINES)
// Copyright 2002 Stephane Marchesin (stephane.marchesin@wanadoo.fr)
// This code is licensed under the LGPL (see COPYING for details)



void SDL_MixAudio_MMX_S16_VC(char* dst,char* src,unsigned int nSize,int volume)
{
	__asm
	{

		push	edi
		push	esi
		push	ebx
		
		mov		edi, dst		
		mov		esi, src		
		mov		eax, volume		
		mov		ebx, nSize		
		shr		ebx, 4			
		jz		endS16
		
		pxor	mm0, mm0
		movd	mm0, eax		
		movq	mm1, mm0		
		psllq	mm0, 16			
		por		mm0, mm1		
		psllq	mm0, 16			
		por		mm0, mm1		
		psllq	mm0, 16			
		por		mm0, mm1		

		#ifndef __WATCOMC__
		align	16
		#endif
mixloopS16:
		movq	mm1, [esi]		
		movq	mm2, mm1		
		movq	mm4, [esi + 8]	
		
		movq	mm7, [edi]		
		
		pmullw	mm1, mm0		
		pmulhw	mm2, mm0		
		movq	mm5, mm4		
		pmullw	mm4, mm0		
		pmulhw	mm5, mm0		
		movq	mm3, mm1		
		punpckhwd	mm1, mm2	
		movq		mm6, mm4	
		punpcklwd	mm3, mm2	
		punpckhwd	mm4, mm5	
		punpcklwd	mm6, mm5	
		
		movq	mm5, [edi + 8]	
		
		psrad	mm1, 7			
		add		esi, 16			
		psrad	mm3, 7			
		psrad	mm4, 7			
		
		packssdw	mm3, mm1	
		psrad	mm6, 7			
		paddsw	mm3, mm7		
		
		packssdw	mm6, mm4	
		movq	[edi], mm3		
		paddsw	mm6, mm5		
		movq	[edi + 8], mm6	
		add		edi, 16			
		dec		ebx				
		jnz mixloopS16

endS16:
		emms
		
		pop		ebx
		pop		esi
		pop		edi
	}

}


void SDL_MixAudio_MMX_S8_VC(char* dst,char* src,unsigned int nSize,int volume)
{
	_asm
	{

		push	edi
		push	esi
		push	ebx
		
		mov		edi, dst	
		mov		esi, src	
		mov		eax, volume	

		movd	mm0, eax	
		movq	mm1, mm0	
		psllq	mm0, 16		
		por		mm0, mm1	
		psllq	mm0, 16		
		por		mm0, mm1	
		psllq	mm0, 16		
		por		mm0, mm1	
		
		mov		ebx, nSize	
		shr		ebx, 3		
		cmp		ebx, 0		
		je		endS8

		#ifndef __WATCOMC__
		align 16
		#endif
mixloopS8:
		pxor	mm2, mm2	
		movq	mm1, [esi]	
		movq	mm3, mm1	
		
		pcmpgtb		mm2, mm1	
		punpckhbw	mm1, mm2	
		punpcklbw	mm3, mm2	
		movq	mm2, [edi]	
		pmullw	mm1, mm0	
		add		esi, 8		
		pmullw	mm3, mm0	
		psraw	mm1, 7		
		psraw	mm3, 7		
		packsswb mm3, mm1	
		paddsb	mm3, mm2	
		movq	[edi], mm3	
		add		edi, 8		
		dec		ebx			
		jnz		mixloopS8
		
endS8:
		emms
		
		pop		ebx
		pop		esi
		pop		edi
	}
}

#endif 
#endif 
