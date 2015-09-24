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
#include "SDL_fbmatrox.h"
#include "matrox_mmio.h"


static void WaitVBL(_THIS)
{
	int count;

	
	mga_waitidle();
	while (  (mga_in8(0x1FDA) & 0x08) )
		;
	while ( !(mga_in8(0x1FDA) & 0x08) )
		; 
	
	count = mga_in32(0x1E20) + 2;
	while ( mga_in32(0x1E20) < count )
		;
}
static void WaitIdle(_THIS)
{
	mga_waitidle();
}

static int SetHWColorKey(_THIS, SDL_Surface *surface, Uint32 key)
{
	return(0);
}

#if 0
static int SetHWAlpha(_THIS, SDL_Surface *surface, Uint8 value)
{
	return(0);
}
#endif

static int FillHWRect(_THIS, SDL_Surface *dst, SDL_Rect *rect, Uint32 color)
{
	int dstX, dstY;
	Uint32 fxbndry;
	Uint32 ydstlen;
	Uint32 fillop;

	
	if ( switched_away ) {
		return -2; 
	}
	if ( dst == this->screen ) {
		SDL_mutexP(hw_lock);
	}

	switch (dst->format->BytesPerPixel) {
	    case 1:
		color |= (color<<8);
	    case 2:
		color |= (color<<16);
		break;
	}

	
	FB_dst_to_xy(this, dst, &dstX, &dstY);

	
	dstX += rect->x;
	dstY += rect->y;

	
	fxbndry = (dstX | ((dstX+rect->w) << 16));

	
	ydstlen = (rect->h | (dstY << 16));

	
	fillop = MGADWG_TRAP | MGADWG_SOLID |
	         MGADWG_ARZERO | MGADWG_SGNZERO | MGADWG_SHIFTZERO;

	
	mga_wait(5);
	mga_out32(MGAREG_DWGCTL, fillop | MGADWG_REPLACE);
	mga_out32(MGAREG_FCOL, color);
	mga_out32(MGAREG_FXBNDRY, fxbndry);
	mga_out32(MGAREG_YDSTLEN + MGAREG_EXEC, ydstlen);

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
	int pitch, w, h;
	int srcX, srcY;
	int dstX, dstY;
	Uint32 sign;
	Uint32 start, stop;
	int skip;
	Uint32 blitop;

	
	if ( dst->pitch != SDL_VideoSurface->pitch ) {
		return(src->map->sw_blit(src, srcrect, dst, dstrect));
	}

	
	if ( switched_away ) {
		return -2; 
	}
	if ( dst == this->screen ) {
		SDL_mutexP(hw_lock);
	}

	
	w = dstrect->w;
	h = dstrect->h;
	FB_dst_to_xy(this, src, &srcX, &srcY);
	FB_dst_to_xy(this, dst, &dstX, &dstY);

	
	srcX += srcrect->x;
	srcY += srcrect->y;
	dstX += dstrect->x;
	dstY += dstrect->y;
	pitch = dst->pitch/dst->format->BytesPerPixel;

	
	sign = 0;
	if ( srcX < dstX ) {
		sign |= 1;
	}
	if ( srcY < dstY ) {
		sign |= 4;
		srcY += (h - 1);
		dstY += (h - 1);
	}

	
	stop = start = (srcY * pitch) + srcX;
	if ( srcX < dstX ) {
		start += (w - 1);
	} else {
		stop  += (w - 1);
	}
	if ( srcY < dstY ) {
		skip = -pitch;
	} else {
		skip = pitch;
	}

	
	if ( (src->flags & SDL_SRCCOLORKEY) == SDL_SRCCOLORKEY ) {
		Uint32 colorkey;

		blitop = MGADWG_BFCOL | MGADWG_BITBLT |
		         MGADWG_SHIFTZERO | MGADWG_RSTR | (0x0C << 16) |
		         MGADWG_TRANSC;

		colorkey = src->format->colorkey;
		switch (dst->format->BytesPerPixel) {
		    case 1:
			colorkey |= (colorkey<<8);
		    case 2:
			colorkey |= (colorkey<<16);
			break;
		}
		mga_wait(2);
		mga_out32(MGAREG_FCOL, colorkey);
		mga_out32(MGAREG_BCOL, 0xFFFFFFFF);
	} else {
		blitop = MGADWG_BFCOL | MGADWG_BITBLT |
		         MGADWG_SHIFTZERO | MGADWG_RSTR | (0x0C << 16);
	}
	mga_wait(7);
	mga_out32(MGAREG_SGN, sign);
	mga_out32(MGAREG_AR3, start);
	mga_out32(MGAREG_AR0, stop);
	mga_out32(MGAREG_AR5, skip);
	mga_out32(MGAREG_FXBNDRY, (dstX | ((dstX + w-1) << 16)));
	mga_out32(MGAREG_YDSTLEN, (dstY << 16) | h);
	mga_out32(MGAREG_DWGCTL + MGAREG_EXEC, blitop);

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

void FB_MatroxAccel(_THIS, __u32 card)
{
	
	this->CheckHWBlit = CheckHWBlit;
	wait_vbl = WaitVBL;
	wait_idle = WaitIdle;

	
	this->info.blit_fill = 1;
	this->FillHWRect = FillHWRect;

	
	this->info.blit_hw = 1;
	if ( card != FB_ACCEL_MATROX_MGA2064W ) {
		this->info.blit_hw_CC = 1;
		this->SetHWColorKey = SetHWColorKey;
	}

#if 0 
	
	if ( (card == FB_ACCEL_MATROX_MGAG200)
	  || (card == FB_ACCEL_MATROX_MGAG400)
	) {
		this->info.blit_hw_A = 1;
		this->SetHWAlpha = SetHWAlpha;
	}
#endif
}
