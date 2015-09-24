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
#include "SDL_fbriva.h"
#include "riva_mmio.h"
#include "riva_regs.h"


static int FifoEmptyCount = 0;
static int FifoFreeCount = 0;

static void WaitVBL(_THIS)
{
	volatile Uint8 *port = (Uint8 *)(mapped_io + PCIO_OFFSET + 0x3DA);

	while (  (*port & 0x08) )
		;
	while ( !(*port & 0x08) )
		;
}
static void NV3WaitIdle(_THIS)
{
	RivaRop *Rop = (RivaRop *)(mapped_io + ROP_OFFSET);
	while ( (Rop->FifoFree < FifoEmptyCount) ||
	        (*(mapped_io + PGRAPH_OFFSET + 0x000006B0) & 0x01) )
		;
}
static void NV4WaitIdle(_THIS)
{
	RivaRop *Rop = (RivaRop *)(mapped_io + ROP_OFFSET);
	while ( (Rop->FifoFree < FifoEmptyCount) ||
	        (*(mapped_io + PGRAPH_OFFSET + 0x00000700) & 0x01) )
		;
}

#if 0 
static int SetHWColorKey(_THIS, SDL_Surface *surface, Uint32 key)
{
	return(0);
}

static int SetHWAlpha(_THIS, SDL_Surface *surface, Uint8 value)
{
	return(0);
}
#endif 

static int FillHWRect(_THIS, SDL_Surface *dst, SDL_Rect *rect, Uint32 color)
{
	int dstX, dstY;
	int dstW, dstH;
	RivaBitmap *Bitmap = (RivaBitmap *)(mapped_io + BITMAP_OFFSET);

	
	if ( switched_away ) {
		return -2; 
	}
	if ( dst == this->screen ) {
		SDL_mutexP(hw_lock);
	}

	
	dstW = rect->w;
	dstH = rect->h;
	FB_dst_to_xy(this, dst, &dstX, &dstY);

	
	dstX += rect->x;
	dstY += rect->y;

	RIVA_FIFO_FREE(Bitmap, 1);
	Bitmap->Color1A = color;

	RIVA_FIFO_FREE(Bitmap, 2);
	Bitmap->UnclippedRectangle[0].TopLeft     = (dstX << 16) | dstY; 
	Bitmap->UnclippedRectangle[0].WidthHeight = (dstW << 16) | dstH;

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
	int srcX, srcY;
	int dstX, dstY;
	int dstW, dstH;
	RivaScreenBlt *Blt = (RivaScreenBlt *)(mapped_io + BLT_OFFSET);

	
	if ( dst->pitch != SDL_VideoSurface->pitch ) {
		return(src->map->sw_blit(src, srcrect, dst, dstrect));
	}

	
	if ( switched_away ) {
		return -2; 
	}
	if ( dst == this->screen ) {
		SDL_mutexP(hw_lock);
	}

	
	dstW = dstrect->w;
	dstH = dstrect->h;
	FB_dst_to_xy(this, src, &srcX, &srcY);
	FB_dst_to_xy(this, dst, &dstX, &dstY);

	
	srcX += srcrect->x;
	srcY += srcrect->y;
	dstX += dstrect->x;
	dstY += dstrect->y;

	RIVA_FIFO_FREE(Blt, 3);
	Blt->TopLeftSrc  = (srcY << 16) | srcX;
	Blt->TopLeftDst  = (dstY << 16) | dstX;
	Blt->WidthHeight = (dstH  << 16) | dstW;

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

void FB_RivaAccel(_THIS, __u32 card)
{
	RivaRop *Rop = (RivaRop *)(mapped_io + ROP_OFFSET);

	
	this->CheckHWBlit = CheckHWBlit;
	wait_vbl = WaitVBL;
	switch (card) {
	    case FB_ACCEL_NV3:
		wait_idle = NV3WaitIdle;
		break;
	    case FB_ACCEL_NV4:
		wait_idle = NV4WaitIdle;
		break;
	    default:
		
		break;
	}
	FifoEmptyCount = Rop->FifoFree;

	
	this->info.blit_fill = 1;
	this->FillHWRect = FillHWRect;

	
	this->info.blit_hw = 1;
#if 0 
	this->info.blit_hw_CC = 1;
	this->SetHWColorKey = SetHWColorKey;
#endif

#if 0 
	
	this->info.blit_hw_A = 1;
	this->SetHWAlpha = SetHWAlpha;
#endif
}
