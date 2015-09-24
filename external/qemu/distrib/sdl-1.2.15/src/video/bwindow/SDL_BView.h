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

#ifndef _SDL_BView_h
#define _SDL_BView_h


extern "C" {
#include "../../events/SDL_events_c.h"
};

class SDL_BView : public BView
{
public:
	SDL_BView(BRect frame) : 
		BView(frame, "SDL View", B_FOLLOW_ALL_SIDES,
					(B_WILL_DRAW|B_FRAME_EVENTS)) {
		image = NULL;
		xoff = yoff = 0;
		SetViewColor(0,0,0,0);
		SetHighColor(0,0,0,0);
	}
	virtual ~SDL_BView() {
		SetBitmap(NULL);
	}
	
	virtual void SetXYOffset(int x, int y) {
		xoff = x;
		yoff = y;
	}
	virtual void GetXYOffset(int &x, int &y) {
		x = xoff;
		y = yoff;
	}
	virtual void GetXYOffset(float &x, float &y) {
		x = (float)xoff;
		y = (float)yoff;
	}
	virtual void FrameResized(float width, float height) {
		BRect bounds;
		bounds.top = bounds.left = 0;
		bounds.right = width;
		bounds.bottom = height;
		 
		FillRect(bounds, B_SOLID_HIGH);
		
		if( image ) {
			bounds = image->Bounds();
			Draw(bounds);
		}
	}
	
	
	virtual void SetBitmap(BBitmap *bitmap) {
		if ( image ) {
			delete image;
		}
		image = bitmap;
	}
	virtual void Draw(BRect updateRect) {
		if ( image ) {
			if(xoff || yoff) {	
				BRect dest;
				dest.top    = updateRect.top + yoff;
				dest.left   = updateRect.left + xoff;
				dest.bottom = updateRect.bottom + yoff;
				dest.right  = updateRect.right + xoff;
				DrawBitmap(image, updateRect, dest);
			} else {
				DrawBitmap(image, updateRect, updateRect);
			}
		}
	}
	virtual void DrawAsync(BRect updateRect) {
		if(xoff || yoff) {	
			BRect dest;
			dest.top    = updateRect.top + yoff;
			dest.left   = updateRect.left + xoff;
			dest.bottom = updateRect.bottom + yoff;
			dest.right  = updateRect.right + xoff;;
			DrawBitmapAsync(image, updateRect, dest);
		} else {
			DrawBitmapAsync(image, updateRect, updateRect);
		}
	}

private:
	BBitmap *image;
	int xoff, yoff;
};

#endif 
