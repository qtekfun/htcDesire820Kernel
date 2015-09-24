/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2012 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "SDL_config.h"


#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

#include "SDL_riscostask.h"
#include "SDL_riscosvideo.h"
#include "SDL_riscosevents_c.h"
#include "SDL_riscosmouse_c.h"

#include "kernel.h"
#include "swis.h"
#include "unixlib/os.h"
#include "unixlib/local.h"

typedef struct tagScreenModeBlock
{
   int flags;  
   int x_pixels;
   int y_pixels;
   int pixel_depth;  
   int frame_rate;   
   int mode_vars[5]; 
} SCREENMODEBLOCK;


void FULLSCREEN_SetDeviceMode(_THIS);
int FULLSCREEN_SetMode(int width, int height, int bpp);
void FULLSCREEN_SetupBanks(_THIS);

static int FULLSCREEN_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors);
static int FULLSCREEN_FlipHWSurface(_THIS, SDL_Surface *surface);
void FULLSCREEN_SetWMCaption(_THIS, const char *title, const char *icon);
extern int RISCOS_GetWmInfo(_THIS, SDL_SysWMinfo *info);

static void FULLSCREEN_UpdateRects(_THIS, int numrects, SDL_Rect *rects);
static void FULLSCREEN_UpdateRectsMemCpy(_THIS, int numrects, SDL_Rect *rects);
static void FULLSCREEN_UpdateRects8bpp(_THIS, int numrects, SDL_Rect *rects);
static void FULLSCREEN_UpdateRects16bpp(_THIS, int numrects, SDL_Rect *rects);
static void FULLSCREEN_UpdateRects32bpp(_THIS, int numrects, SDL_Rect *rects);
static void FULLSCREEN_UpdateRectsOS(_THIS, int numrects, SDL_Rect *rects);

static int cmpmodes(const void *va, const void *vb);
static int FULLSCREEN_AddMode(_THIS, int bpp, int w, int h);
void FULLSCREEN_SetWriteBank(int bank);
void FULLSCREEN_SetDisplayBank(int bank);
static void FULLSCREEN_DisableEscape();
static void FULLSCREEN_EnableEscape();
void FULLSCREEN_BuildModeList(_THIS);

extern int riscos_backbuffer; 

extern unsigned char *WIMP_CreateBuffer(int width, int height, int bpp);

extern void RISCOS_Put32(void *to, int pixels, int pitch, int rows, void *from, int src_skip_bytes);

SDL_Surface *FULLSCREEN_SetVideoMode(_THIS, SDL_Surface *current,
				int width, int height, int bpp, Uint32 flags)
{
   _kernel_swi_regs regs;
   Uint32 Rmask = 0;
   Uint32 Gmask = 0;
   Uint32 Bmask = 0;
   int create_back_buffer = riscos_backbuffer;

   switch(bpp)
   {
	case 8:
		flags |= SDL_HWPALETTE;
		break;

	case 15:
	case 16:
		Bmask = 0x00007c00;
		Gmask = 0x000003e0;
		Rmask = 0x0000001f;
		break;

	case 32:
		Bmask = 0x00ff0000;
		Gmask = 0x0000ff00;
		Rmask = 0x000000ff;
		break;

	default:
		SDL_SetError("Pixel depth not supported");
		return NULL;
		break;
   }

   if (FULLSCREEN_SetMode(width, height, bpp) == 0)
   {
	   SDL_SetError("Couldn't set requested mode");
	   return (NULL);
   }


	
	if ( ! SDL_ReallocFormat(current, bpp, Rmask, Gmask, Bmask, 0) ) {
	    RISCOS_RestoreWimpMode();
		SDL_SetError("Couldn't allocate new pixel format for requested mode");
		return(NULL);
	}

	
	current->w = width;
	this->hidden->height = current->h = height;

   regs.r[0] = -1; 

   
   regs.r[1] = 6; 
   _kernel_swi(OS_ReadModeVariable, &regs, &regs);

   current->pitch = regs.r[2];

   if (flags & SDL_DOUBLEBUF)
   {
	   regs.r[0] = 2; 
	   _kernel_swi(OS_ReadDynamicArea, &regs, &regs);
	   
	   
	   regs.r[0] = 2; 
	   regs.r[1] = (current->pitch * height * 2) - regs.r[1];
	   if (_kernel_swi(OS_ChangeDynamicArea, &regs, &regs) != NULL)
	   {
		   
		   flags &= ~SDL_DOUBLEBUF;
	   }
   }
   
  	current->flags = flags | SDL_FULLSCREEN | SDL_HWSURFACE | SDL_PREALLOC;
	

	
	if (flags & SDL_DOUBLEBUF)
	{
	   FULLSCREEN_SetWriteBank(0);
	   FULLSCREEN_SetDisplayBank(1);

         create_back_buffer = 0; 
    }

    FULLSCREEN_SetupBanks(this);

    if (create_back_buffer)
    {
       if (riscos_backbuffer == 3)
          this->hidden->bank[0] = WIMP_CreateBuffer(width, height, bpp);
       else
          this->hidden->bank[0] = SDL_malloc(height * current->pitch);
       if (this->hidden->bank[0] == 0)
       {
 	       RISCOS_RestoreWimpMode();
           SDL_SetError("Couldnt allocate memory for back buffer");
           return (NULL);
       }
       
       current->flags &= ~SDL_HWSURFACE;
    }

    
    if (this->hidden->alloc_bank) SDL_free(this->hidden->alloc_bank);
    if (create_back_buffer)
    {
        this->hidden->alloc_bank = this->hidden->bank[0];
        if (riscos_backbuffer == 3)
        {
           this->hidden->bank[0] += 60; 
           if (bpp == 8) this->hidden->bank[0] += 2048; 
        }
    } else
	  this->hidden->alloc_bank = 0;

    
    SDL_memset(this->hidden->bank[0], 0, height * current->pitch);
    SDL_memset(this->hidden->bank[1], 0, height * current->pitch);

 	   this->hidden->current_bank = 0;
	   current->pixels = this->hidden->bank[0];

    
    this->screen = current;

	
	FULLSCREEN_SetDeviceMode(this);


	
	return(current);
}

void FULLSCREEN_SetDeviceMode(_THIS)
{
	

	if (riscos_backbuffer && (this->screen->flags & SDL_DOUBLEBUF) == 0)
      {
	   switch(riscos_backbuffer)
         {
            case 2: 
               switch(this->screen->format->BytesPerPixel)
               {
                  case 1: 
               	   this->UpdateRects = FULLSCREEN_UpdateRects8bpp;
                     break;
                  case 2: 
               	   this->UpdateRects = FULLSCREEN_UpdateRects16bpp;
                     break;
                  case 4: 
               	   this->UpdateRects = FULLSCREEN_UpdateRects32bpp;
                     break;

                  default: 
               	   this->UpdateRects = FULLSCREEN_UpdateRectsMemCpy;
                     break;
                }
               break;

            case 3: 
               this->UpdateRects = FULLSCREEN_UpdateRectsOS;
               break;

            default: 
               this->UpdateRects = FULLSCREEN_UpdateRectsMemCpy;
               break;
         }
      } else
	   this->UpdateRects = FULLSCREEN_UpdateRects; 

	this->SetColors   = FULLSCREEN_SetColors;

	this->FlipHWSurface = FULLSCREEN_FlipHWSurface;

	this->SetCaption = FULLSCREEN_SetWMCaption;
	this->SetIcon = NULL;
	this->IconifyWindow = NULL;
	
	this->ShowWMCursor = RISCOS_ShowWMCursor;
	this->WarpWMCursor = FULLSCREEN_WarpWMCursor;

	this->PumpEvents = FULLSCREEN_PumpEvents;	
}

void FULLSCREEN_BuildModeList(_THIS)
{
	_kernel_swi_regs regs;
	char *enumInfo = NULL;
	char *enum_ptr;
	int *blockInfo;
	int j;
	int num_modes;

	
	regs.r[0] = 2; 
	regs.r[2] = 0; 
	regs.r[6] = 0; 
	regs.r[7] = 0; 
	_kernel_swi(OS_ScreenMode, &regs, &regs);

    num_modes = -regs.r[2];

	
	this->info.video_mem = regs.r[5]/1024;

	enumInfo = (unsigned char *)SDL_malloc(-regs.r[7]);
	if (enumInfo == NULL)
	{
		SDL_OutOfMemory();
		return;
	}
	
	regs.r[2] = 0;
	regs.r[6] = (int)enumInfo;
	regs.r[7] = -regs.r[7];
	_kernel_swi(OS_ScreenMode, &regs, &regs);

	enum_ptr = enumInfo;

	for (j =0; j < num_modes;j++)
	{
		blockInfo = (int *)enum_ptr;
		if ((blockInfo[1] & 255) == 1) 
		{
			switch(blockInfo[4])
			{
			case 3: 
				FULLSCREEN_AddMode(this, 8, blockInfo[2], blockInfo[3]);
				break;
			case 4: 
				FULLSCREEN_AddMode(this, 15, blockInfo[2], blockInfo[3]);
				break;
			case 5: 
				FULLSCREEN_AddMode(this, 32, blockInfo[2], blockInfo[3]);
				break;
			}
		}

		enum_ptr += blockInfo[0];
	}

	SDL_free(enumInfo);
		
	
	for ( j=0; j<NUM_MODELISTS; ++j ) {
		if ( SDL_nummodes[j] > 0 ) {
			SDL_qsort(SDL_modelist[j], SDL_nummodes[j], sizeof *SDL_modelist[j], cmpmodes);
		}
	}
}

static int FULLSCREEN_FlipHWSurface(_THIS, SDL_Surface *surface)
{
   _kernel_swi_regs regs;
   regs.r[0] = 19;

   FULLSCREEN_SetDisplayBank(this->hidden->current_bank);
   this->hidden->current_bank ^= 1;
   FULLSCREEN_SetWriteBank(this->hidden->current_bank);
   surface->pixels = this->hidden->bank[this->hidden->current_bank];

   
   _kernel_swi(OS_Byte, &regs, &regs);

	return(0);
}

static void FULLSCREEN_UpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
}

static void FULLSCREEN_UpdateRectsMemCpy(_THIS, int numrects, SDL_Rect *rects)
{
      int j;
      char *to, *from;
      int pitch = this->screen->pitch;
      int row;
      int xmult = this->screen->format->BytesPerPixel;
      for (j = 0; j < numrects; j++)
      {
         from = this->hidden->bank[0] + rects->x * xmult + rects->y * pitch;
         to  = this->hidden->bank[1] + rects->x * xmult + rects->y * pitch;
         for (row = 0; row < rects->h; row++)
         {
             SDL_memcpy(to, from, rects->w * xmult);
             from += pitch;
             to += pitch;
         }
         rects++;
      }
}

static void FULLSCREEN_UpdateRects8bpp(_THIS, int numrects, SDL_Rect *rects)
{
   int j;
   char *to, *from;
   int pitch = this->screen->pitch;
   int width_bytes;
   int src_skip_bytes;

   for (j = 0; j < numrects; j++)
   {
      from = this->hidden->bank[0] + rects->x + rects->y * pitch;
      to  = this->hidden->bank[1] + rects->x + rects->y * pitch;
      width_bytes = rects->w;
      if ((int)from & 3)
      {
         int extra = ((int)from & 3);
         from -= extra;
         to -= extra;
         width_bytes += extra;
      }
      if (width_bytes & 3) width_bytes += 4 - (width_bytes & 3);
      src_skip_bytes = pitch - width_bytes;
               
      RISCOS_Put32(to, (width_bytes >> 2), pitch, (int)rects->h, from, src_skip_bytes);
      rects++;
   }
}

static void FULLSCREEN_UpdateRects16bpp(_THIS, int numrects, SDL_Rect *rects)
{
   int j;
   char *to, *from;
   int pitch = this->screen->pitch;
   int width_bytes;
   int src_skip_bytes;

   for (j = 0; j < numrects; j++)
   {
      from = this->hidden->bank[0] + (rects->x << 1) + rects->y * pitch;
      to  = this->hidden->bank[1] + (rects->x << 1) + rects->y * pitch;
      width_bytes = (((int)rects->w) << 1);
      if ((int)from & 3)
      {
         from -= 2;
         to -= 2;
         width_bytes += 2;
      }
      if (width_bytes & 3) width_bytes += 2;
      src_skip_bytes = pitch - width_bytes;
               
      RISCOS_Put32(to, (width_bytes >> 2), pitch, (int)rects->h, from, src_skip_bytes);
      rects++;
   }
}

static void FULLSCREEN_UpdateRects32bpp(_THIS, int numrects, SDL_Rect *rects)
{
   int j;
   char *to, *from;
   int pitch = this->screen->pitch;
   int width;

   for (j = 0; j < numrects; j++)
   {
      from = this->hidden->bank[0] + (rects->x << 2) + rects->y * pitch;
      to  = this->hidden->bank[1] + (rects->x << 2) + rects->y * pitch;
      width = (int)rects->w ;
               
      RISCOS_Put32(to, width, pitch, (int)rects->h, from, pitch - (width << 2));
      rects++;
   }
}

static void FULLSCREEN_UpdateRectsOS(_THIS, int numrects, SDL_Rect *rects)
{
   _kernel_swi_regs regs;
   _kernel_oserror *err;
   int j;
   int y;

   regs.r[0] = 28 + 512;
   regs.r[1] = (unsigned int)this->hidden->alloc_bank;
   regs.r[2] = (unsigned int)this->hidden->alloc_bank+16;
   regs.r[5] = 0;

   for (j = 0; j < numrects; j++)
   {
      y = this->screen->h - rects->y; 
      _kernel_oswrch(24); 
      _kernel_oswrch((rects->x << this->hidden->xeig) & 0xFF); 
      _kernel_oswrch(((rects->x << this->hidden->xeig) >> 8) & 0xFF);
      _kernel_oswrch(((y - rects->h) << this->hidden->yeig) & 0xFF); 
      _kernel_oswrch((((y - rects->h) << this->hidden->yeig)>> 8) & 0xFF);
      _kernel_oswrch(((rects->x + rects->w - 1) << this->hidden->xeig) & 0xFF); 
      _kernel_oswrch((((rects->x + rects->w - 1)<< this->hidden->xeig) >> 8) & 0xFF);
      _kernel_oswrch(((y-1) << this->hidden->yeig) & 0xFF); 
      _kernel_oswrch((((y-1) << this->hidden->yeig) >> 8) & 0xFF);

      regs.r[3] = 0;
      regs.r[4] = 0;

      if ((err = _kernel_swi(OS_SpriteOp, &regs, &regs)) != 0)
      {
         printf("OS_SpriteOp failed \n%s\n",err->errmess);
      }

      rects++;

      
      _kernel_oswrch(24); 
      _kernel_oswrch(0); 
      _kernel_oswrch(0);
      _kernel_oswrch(0); 
      _kernel_oswrch(0);
      _kernel_oswrch(((this->screen->w-1) << this->hidden->xeig) & 0xFF); 
      _kernel_oswrch((((this->screen->w-1) << this->hidden->xeig) >> 8) & 0xFF);
      _kernel_oswrch(((this->screen->h-1) << this->hidden->yeig) & 0xFF); 
      _kernel_oswrch((((this->screen->h-1) << this->hidden->yeig) >> 8) & 0xFF);
   }
}


int FULLSCREEN_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
	_kernel_swi_regs regs;
	int palette[256];

	regs.r[0] = -1;
	regs.r[1] = -1;
	regs.r[2] = (int)palette;
	regs.r[3] = 1024;
	regs.r[4] = 0;
	_kernel_swi(ColourTrans_ReadPalette, &regs, &regs);

	while(ncolors--)
	{
		palette[firstcolor] = ((colors->b) << 24) | ((colors->g) << 16) | ((colors->r) << 8);
		firstcolor++;
		colors++;
	}

	regs.r[0] = -1;
	regs.r[1] = -1;
	regs.r[2] = (int)palette;
	regs.r[3] = 0;
	regs.r[4] = 0;
	_kernel_swi(ColourTrans_WritePalette, &regs, &regs);

	return(1);
}


static int cmpmodes(const void *va, const void *vb)
{
    SDL_Rect *a = *(SDL_Rect **)va;
    SDL_Rect *b = *(SDL_Rect **)vb;
    if(a->w == b->w)
        return b->h - a->h;
    else
        return b->w - a->w;
}

static int FULLSCREEN_AddMode(_THIS, int bpp, int w, int h)
{
	SDL_Rect *mode;
	int i, index;
	int next_mode;

	
	if ( bpp < 8 ) {  
		return(0);
	}
	index = ((bpp+7)/8)-1;
	for ( i=0; i<SDL_nummodes[index]; ++i ) {
		mode = SDL_modelist[index][i];
		if ( (mode->w == w) && (mode->h == h) ) {
			return(0);
		}
	}

	
	mode = (SDL_Rect *)SDL_malloc(sizeof *mode);
	if ( mode == NULL ) {
		SDL_OutOfMemory();
		return(-1);
	}
	mode->x = 0;
	mode->y = 0;
	mode->w = w;
	mode->h = h;

	
	next_mode = SDL_nummodes[index];
	SDL_modelist[index] = (SDL_Rect **)
	       SDL_realloc(SDL_modelist[index], (1+next_mode+1)*sizeof(SDL_Rect *));
	if ( SDL_modelist[index] == NULL ) {
		SDL_OutOfMemory();
		SDL_nummodes[index] = 0;
		SDL_free(mode);
		return(-1);
	}
	SDL_modelist[index][next_mode] = mode;
	SDL_modelist[index][next_mode+1] = NULL;
	SDL_nummodes[index]++;

	return(0);
}

void FULLSCREEN_SetWriteBank(int bank)
{
   _kernel_swi_regs regs;
   regs.r[0] = 112;
   regs.r[1] = bank+1;
   _kernel_swi(OS_Byte, &regs, &regs);
}

void FULLSCREEN_SetDisplayBank(int bank)
{
   _kernel_swi_regs regs;
   regs.r[0] = 113;
   regs.r[1] = bank+1;
   _kernel_swi(OS_Byte, &regs, &regs);
}


static void FULLSCREEN_DisableEscape()
{
   _kernel_swi_regs regs;
   regs.r[0] = 229;
   regs.r[1] = 1;
   regs.r[2] = 0;
   _kernel_swi(OS_Byte, &regs, &regs);
  
}

static void FULLSCREEN_EnableEscape()
{
   _kernel_swi_regs regs;
   regs.r[0] = 229;
   regs.r[1] = 0;
   regs.r[2] = 0;
   _kernel_swi(OS_Byte, &regs, &regs);
  
}

void FULLSCREEN_SetWMCaption(_THIS, const char *title, const char *icon)
{
	SDL_strlcpy(this->hidden->title, title, SDL_arraysize(this->hidden->title));
}


int FULLSCREEN_SetMode(int width, int height, int bpp)
{
   SCREENMODEBLOCK smb;
   _kernel_swi_regs regs;

   smb.flags = 1;
   smb.x_pixels = width;
   smb.y_pixels = height;
   smb.mode_vars[0] = -1;

   switch(bpp)
   {
	case 8:
		smb.pixel_depth = 3;
		
		smb.mode_vars[0] = 0; smb.mode_vars[1] = 128; 
		smb.mode_vars[2] = 3; smb.mode_vars[3] = 255; 
		smb.mode_vars[4] = -1; 
		break;

	case 15:
	case 16:
		smb.pixel_depth = 4;
		break;

	case 32:
		smb.pixel_depth = 5;
		break;

	default:
		SDL_SetError("Pixel depth not supported");
		return 0;
		break;
   }
   
   smb.frame_rate = -1;

   regs.r[0] = 0;
   regs.r[1] = (int)&smb;

   if (_kernel_swi(OS_ScreenMode, &regs, &regs) != 0)
   {
	   SDL_SetError("Couldn't set requested mode");
	   return 0;
   }

    
    _kernel_oswrch(23);_kernel_oswrch(1);_kernel_oswrch(0);
    _kernel_oswrch(0);_kernel_oswrch(0);_kernel_oswrch(0);
    _kernel_oswrch(0);_kernel_oswrch(0);_kernel_oswrch(0);
    _kernel_oswrch(0);_kernel_oswrch(0);

   return 1;
}

void FULLSCREEN_SetupBanks(_THIS)
{
   _kernel_swi_regs regs;
   int block[5];
   block[0] = 148; 
   block[1] = 149; 
   block[2] = 4;  
   block[3] = 5;  
   block[4] = -1;  

   regs.r[0] = (int)block;
   regs.r[1] = (int)block;
   _kernel_swi(OS_ReadVduVariables, &regs, &regs);

   this->hidden->bank[0] = (void *)block[0];
   this->hidden->bank[1] = (void *)block[1];
   this->hidden->xeig = block[2];
   this->hidden->yeig = block[3];
}


int FULLSCREEN_ToggleFromWimp(_THIS)
{
   int width = this->screen->w;
   int height = this->screen->h;
   int bpp = this->screen->format->BitsPerPixel;

   RISCOS_StoreWimpMode();
   if (FULLSCREEN_SetMode(width, height, bpp))
   {
       char *buffer = this->hidden->alloc_bank; 
       
       if (riscos_backbuffer == 0) riscos_backbuffer = 1;

       FULLSCREEN_SetupBanks(this);

       this->hidden->bank[0] = buffer + 60; 
       if (bpp == 8) this->hidden->bank[0] += 2048; 

	   this->hidden->current_bank = 0;
	   this->screen->pixels = this->hidden->bank[0];

       
       SDL_memcpy(this->hidden->bank[1], this->hidden->bank[0], width * height * this->screen->format->BytesPerPixel);

       FULLSCREEN_SetDeviceMode(this);
       return 1;
   } else
      RISCOS_RestoreWimpMode();

   return 0;
}
