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
#include "../SDL_blit.h"
#include "SDL_fb3dfx.h"
#include "3dfx_mmio.h"


static void WaitVBL(_THIS)
{
	
	tdfx_waitidle();
	while( (tdfx_in32(TDFX_STATUS) & STATUS_RETRACE) == STATUS_RETRACE )
		;
	
	while( (tdfx_in32(TDFX_STATUS) & STATUS_RETRACE) == 0 )
		; 
}
static void WaitIdle(_THIS)
{
	tdfx_waitidle();
}

static int SetHWColorKey(_THIS, SDL_Surface *surface, Uint32 key)
{
	return(0);
}

static int FillHWRect(_THIS, SDL_Surface *dst, SDL_Rect *rect, Uint32 color)
{
	int bpp;
	Uint32 dst_base;
	Uint32 format;
	int dstX, dstY;

	
	if ( switched_away ) {
		return -2; 
	}
	if ( dst == this->screen ) {
		SDL_mutexP(hw_lock);
	}

	
	dst_base = ((char *)dst->pixels - mapped_mem);
	bpp = dst->format->BitsPerPixel;
	format = dst->pitch | ((bpp+((bpp==8) ? 0 : 8)) << 13);

	
	dstX = rect->x;
	dstY = rect->y;

	
	tdfx_wait(6);
	tdfx_out32(DSTBASE, dst_base);
	tdfx_out32(DSTFORMAT, format);
	tdfx_out32(COLORFORE, color);
	tdfx_out32(COMMAND_2D, COMMAND_2D_FILLRECT);
	tdfx_out32(DSTSIZE, rect->w | (rect->h << 16));
	tdfx_out32(LAUNCH_2D, dstX | (dstY << 16));

	FB_AddBusySurface(dst);

	if ( dst == this->screen ) {
		SDL_mutexV(hw_lock);
	}
	return(0);
}

static int HWAccelBlit(SDL_Surface *src, SDL_Rect *srcrect,
                       SDL_Surface *dst, SDL_Rect *dstrect)
{
	SDL_VideoDevice *this = current_video;
	int bpp;
	Uint32 src_format;
	Uint32 src_base;
	Uint32 dst_base;
	int srcX, srcY;
	int dstX, dstY;
	Uint32 blitop;
	Uint32 use_colorkey;

	
	if ( switched_away ) {
		return -2; 
	}
	if ( dst == this->screen ) {
		SDL_mutexP(hw_lock);
	}

	
	src_base = ((char *)src->pixels - mapped_mem);
	bpp = src->format->BitsPerPixel;
	src_format = src->pitch | ((bpp+((bpp==8) ? 0 : 8)) << 13);
	dst_base = ((char *)dst->pixels - mapped_mem);
	bpp = dst->format->BitsPerPixel;

	srcX = srcrect->x;
	srcY = srcrect->y;
	dstX = dstrect->x;
	dstY = dstrect->y;

	
	blitop = COMMAND_2D_BITBLT | (0xCC << 24);
	if ( srcX <= dstX ) {
		blitop |= BIT(14);
		srcX += (dstrect->w - 1);
		dstX += (dstrect->w - 1);
	}
	if ( srcY <= dstY ) {
		blitop |= BIT(15);
		srcY += (dstrect->h - 1);
		dstY += (dstrect->h - 1);
	}

	
	if ( (src->flags & SDL_SRCCOLORKEY) == SDL_SRCCOLORKEY ) {
    		tdfx_wait(3);
    		tdfx_out32(SRCCOLORKEYMIN, src->format->colorkey);
    		tdfx_out32(SRCCOLORKEYMAX, src->format->colorkey);
    		tdfx_out32(ROP_2D, 0xAA00);
		use_colorkey = 1;
	} else {
		use_colorkey = 0;
	}
	tdfx_wait(9);
	tdfx_out32(SRCBASE, (Uint32)src_base);
	tdfx_out32(SRCFORMAT, src_format);
	tdfx_out32(DSTBASE, (Uint32)dst_base);
	tdfx_out32(DSTFORMAT, src_format);
	tdfx_out32(COMMAND_2D, blitop);
	tdfx_out32(COMMANDEXTRA_2D, use_colorkey);
	tdfx_out32(DSTSIZE, dstrect->w | (dstrect->h << 16));
	tdfx_out32(DSTXY, dstX | (dstY << 16));
	tdfx_out32(LAUNCH_2D, srcX | (srcY << 16));

	FB_AddBusySurface(src);
	FB_AddBusySurface(dst);

	if ( dst == this->screen ) {
		SDL_mutexV(hw_lock);
	}
	return(0);
}

static int CheckHWBlit(_THIS, SDL_Surface *src, SDL_Surface *dst)
{
	int accelerated;

	
	src->flags |= SDL_HWACCEL;

	
	if ( (src->flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		if ( ! this->info.blit_hw_A ) {
			src->flags &= ~SDL_HWACCEL;
		}
	}
	if ( (src->flags & SDL_SRCCOLORKEY) == SDL_SRCCOLORKEY ) {
		if ( ! this->info.blit_hw_CC ) {
			src->flags &= ~SDL_HWACCEL;
		}
	}

	
	accelerated = !!(src->flags & SDL_HWACCEL);
	if ( accelerated ) {
		src->map->hw_blit = HWAccelBlit;
	}
	return(accelerated);
}

void FB_3DfxAccel(_THIS, __u32 card)
{
	
	this->CheckHWBlit = CheckHWBlit;
	wait_vbl = WaitVBL;
	wait_idle = WaitIdle;

	
	tdfx_out32(BRESERROR0, 0);
	tdfx_out32(BRESERROR1, 0);

	
	this->info.blit_fill = 1;
	this->FillHWRect = FillHWRect;

	
	this->info.blit_hw = 1;
	this->info.blit_hw_CC = 1;
	this->SetHWColorKey = SetHWColorKey;
}
