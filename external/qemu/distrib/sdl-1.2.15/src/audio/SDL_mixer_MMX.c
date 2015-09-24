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

/*
    MMX assembler version of SDL_MixAudio for signed little endian 16 bit samples and signed 8 bit samples
    Copyright 2002 Stephane Marchesin (stephane.marchesin@wanadoo.fr)
    This code is licensed under the LGPL (see COPYING for details)
 
    Assumes buffer size in bytes is a multiple of 16
    Assumes SDL_MIX_MAXVOLUME = 128
*/



#if defined(SDL_BUGGY_MMX_MIXERS) 
#if defined(__GNUC__) && defined(__i386__) && defined(SDL_ASSEMBLY_ROUTINES)
void SDL_MixAudio_MMX_S16(char* dst,char* src,unsigned int size,int volume)
{
    __asm__ __volatile__ (

"	movl %3,%%eax\n"	

"	movl %2,%%edx\n"	

"	shrl $4,%%edx\n"	

"	jz .endS16\n"

"	pxor %%mm0,%%mm0\n"

"	movd %%eax,%%mm0\n"
"	movq %%mm0,%%mm1\n"
"	psllq $16,%%mm0\n"
"	por %%mm1,%%mm0\n"
"	psllq $16,%%mm0\n"
"	por %%mm1,%%mm0\n"
"	psllq $16,%%mm0\n"
"	por %%mm1,%%mm0\n"		

".align 8\n"
"	.mixloopS16:\n"

"	movq (%1),%%mm1\n" 

"	movq %%mm1,%%mm2\n" 

"	movq 8(%1),%%mm4\n" 

	
"	movq (%0),%%mm7\n" 

	
"	pmullw %%mm0,%%mm1\n" 

"	pmulhw %%mm0,%%mm2\n" 
"	movq %%mm4,%%mm5\n" 

"	pmullw %%mm0,%%mm4\n" 

"	pmulhw %%mm0,%%mm5\n" 
"	movq %%mm1,%%mm3\n" 

"	punpckhwd %%mm2,%%mm1\n" 

"	movq %%mm4,%%mm6\n" 
"	punpcklwd %%mm2,%%mm3\n" 

"	punpckhwd %%mm5,%%mm4\n" 

"	punpcklwd %%mm5,%%mm6\n" 

	
"	movq 8(%0),%%mm5\n" 

	
"	psrad $7,%%mm1\n" 
"	add $16,%1\n"

"	psrad $7,%%mm3\n" 

"	psrad $7,%%mm4\n" 

	
"	packssdw %%mm1,%%mm3\n" 

"	psrad $7,%%mm6\n" 
"	paddsw %%mm7,%%mm3\n" 

	
"	packssdw %%mm4,%%mm6\n" 
"	movq %%mm3,(%0)\n"

"	paddsw %%mm5,%%mm6\n" 

"	movq %%mm6,8(%0)\n"

"	add $16,%0\n"

"	dec %%edx\n"

"	jnz .mixloopS16\n"

"	emms\n"

".endS16:\n"
	 :
	 : "r" (dst), "r"(src),"m"(size),
	 "m"(volume)
	 : "eax","edx","memory"
	 );
}




void SDL_MixAudio_MMX_S8(char* dst,char* src,unsigned int size,int volume)
{
    __asm__ __volatile__ (

"	movl %3,%%eax\n"	

"	movd %%eax,%%mm0\n"
"	movq %%mm0,%%mm1\n"
"	psllq $16,%%mm0\n"
"	por %%mm1,%%mm0\n"
"	psllq $16,%%mm0\n"
"	por %%mm1,%%mm0\n"
"	psllq $16,%%mm0\n"
"	por %%mm1,%%mm0\n"

"	movl %2,%%edx\n"	
"	shr $3,%%edx\n"	

"	cmp $0,%%edx\n"
"	je .endS8\n"

".align 8\n"
"	.mixloopS8:\n"

"	pxor %%mm2,%%mm2\n"		
"	movq (%1),%%mm1\n"	

"	movq %%mm1,%%mm3\n" 	

	
"	pcmpgtb %%mm1,%%mm2\n"	

"	punpckhbw %%mm2,%%mm1\n"	

"	punpcklbw %%mm2,%%mm3\n"	
"	movq (%0),%%mm2\n"	

"	pmullw %%mm0,%%mm1\n"	
"	add $8,%1\n"

"	pmullw %%mm0,%%mm3\n"	
"	psraw $7,%%mm1\n"		

"	psraw $7,%%mm3\n"		

"	packsswb %%mm1,%%mm3\n"	

"	paddsb %%mm2,%%mm3\n"	

"	movq %%mm3,(%0)\n"	
"	add $8,%0\n"

"	dec %%edx\n"

"	jnz .mixloopS8\n"

".endS8:\n"
"	emms\n"
	 :
	 : "r" (dst), "r"(src),"m"(size),
	 "m"(volume)
	 : "eax","edx","memory"
	 );
}
#endif
#endif
