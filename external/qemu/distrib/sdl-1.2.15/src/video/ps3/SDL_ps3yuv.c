/*
 * SDL - Simple DirectMedia Layer
 * CELL BE Support for PS3 Framebuffer
 * Copyright (C) 2008, 2009 International Business Machines Corporation
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
 * USA
 *
 *  Martin Lowinski  <lowinski [at] de [dot] ibm [ibm] com>
 *  Dirk Herrendoerfer <d.herrendoerfer [at] de [dot] ibm [dot] com>
 *  SPE code based on research by:
 *  Rene Becker
 *  Thimo Emmerich
 */

#include "SDL_config.h"

#include "SDL_video.h"
#include "SDL_ps3video.h"
#include "SDL_ps3yuv_c.h"
#include "../SDL_yuvfuncs.h"
#include "spulibs/spu_common.h"

extern spe_program_handle_t yuv2rgb_spu;
extern spe_program_handle_t bilin_scaler_spu;

int SPE_Start(_THIS, spu_data_t * spe_data);
int SPE_Stop(_THIS, spu_data_t * spe_data);
int SPE_Boot(_THIS, spu_data_t * spe_data);
int SPE_Shutdown(_THIS, spu_data_t * spe_data);
int SPE_SendMsg(_THIS, spu_data_t * spe_data, unsigned int msg);
int SPE_WaitForMsg(_THIS, spu_data_t * spe_data, unsigned int msg);
void SPE_RunContext(void *thread_argp);


static struct private_yuvhwfuncs ps3_yuvfuncs = {
  PS3_LockYUVOverlay,
  PS3_UnlockYUVOverlay,
  PS3_DisplayYUVOverlay,
  PS3_FreeYUVOverlay
};


struct private_yuvhwdata {
	SDL_Surface *display;
	SDL_Surface *stretch;
    volatile void * pixels __attribute__((aligned(128)));

	
	Uint16 pitches[3];
	Uint8 * planes[3];

	unsigned int scale;

	
	Uint8 * scaler_out __attribute__((aligned(128)));

	
    volatile struct yuv2rgb_parms_t * converter_parms __attribute__((aligned(128)));

	
    volatile struct scale_parms_t * scaler_parms __attribute__((aligned(128)));

	Uint8 locked;
};


SDL_Overlay *PS3_CreateYUVOverlay(_THIS, int width, int height, Uint32 format, SDL_Surface *display) {
	
	if ((display->format->BytesPerPixel != 2) &&
			(display->format->BytesPerPixel != 3) &&
			(display->format->BytesPerPixel != 4))
	{
		SDL_SetError ("Can't use YUV data on non 16/24/32 bit surfaces");
		return NULL;
	}

	
	switch (format) {
	    case SDL_IYUV_OVERLAY:
		case SDL_YV12_OVERLAY:
			
			break;
		default:
			SDL_SetError("Unsupported YUV format");
			return NULL;
	}

	SDL_Overlay* overlay;
	struct private_yuvhwdata* hwdata;

	
	overlay = (SDL_Overlay *) SDL_calloc(1, sizeof(SDL_Overlay));
	if (overlay == NULL) {
		SDL_OutOfMemory();
		return NULL;
	}
	SDL_memset(overlay, 0, (sizeof *overlay));

	
	overlay->format = format;
	overlay->w = width;
	overlay->h = height;
	overlay->hwdata = NULL;

	
	overlay->hwfuncs = &ps3_yuvfuncs;

	
	hwdata = (struct private_yuvhwdata *) SDL_calloc(1, sizeof(struct private_yuvhwdata));
	if (hwdata == NULL) {
		SDL_OutOfMemory();
		SDL_FreeYUVOverlay(overlay);
		return NULL;
	}
	overlay->hwdata = hwdata;

	hwdata->stretch = NULL;
	hwdata->display = display;

	
	hwdata->converter_parms = (struct yuv2rgb_parms_t *) memalign(16, sizeof(struct yuv2rgb_parms_t));
	hwdata->scaler_parms = (struct scale_parms_t *) memalign(16, sizeof(struct scale_parms_t));
	if (hwdata->converter_parms == NULL || hwdata->scaler_parms == NULL) {
		SDL_FreeYUVOverlay(overlay);
		SDL_OutOfMemory();
		return(NULL);
	}

	
	scaler_thread_data = (spu_data_t *) malloc(sizeof(spu_data_t));
	converter_thread_data = (spu_data_t *) malloc(sizeof(spu_data_t));
	if (converter_thread_data == NULL || scaler_thread_data == NULL) {
		SDL_FreeYUVOverlay(overlay);
		SDL_OutOfMemory();
		return(NULL);
	}

	scaler_thread_data->program = bilin_scaler_spu;
	scaler_thread_data->program_name = "bilin_scaler_spu";
	scaler_thread_data->keepalive = 0;
	scaler_thread_data->booted = 0;

	converter_thread_data->program = yuv2rgb_spu;
	converter_thread_data->program_name = "yuv2rgb_spu";
	converter_thread_data->keepalive = 1;
	converter_thread_data->booted = 0;

	SPE_Start(this, converter_thread_data);

	hwdata->pixels = (Uint8 *) memalign(16, width * height + ((width * height) >> 1));
	if (hwdata->pixels == NULL) {
		SDL_FreeYUVOverlay(overlay);
		SDL_OutOfMemory();
		return(NULL);
	}

	
	overlay->pitches = hwdata->pitches;
	overlay->pixels = hwdata->planes;
	switch (format) {
	    case SDL_YV12_OVERLAY:
	    case SDL_IYUV_OVERLAY:
			overlay->pitches[0] = overlay->w;
			overlay->pitches[1] = overlay->pitches[0] / 2;
			overlay->pitches[2] = overlay->pitches[0] / 2;
			overlay->pixels[0] = (Uint8 *)hwdata->pixels;
			overlay->pixels[1] = overlay->pixels[0] +
				overlay->pitches[0] * overlay->h;
			overlay->pixels[2] = overlay->pixels[1] +
				overlay->pitches[1] * overlay->h / 2;
			overlay->planes = 3;
		break;
	    default:
		
		break;
	}

	
	return overlay;
}


int PS3_LockYUVOverlay(_THIS, SDL_Overlay *overlay) {
	if (overlay == NULL) {
		return -1;
	}
	overlay->hwdata->locked = 1;

	return 0;
}


void PS3_UnlockYUVOverlay(_THIS, SDL_Overlay *overlay) {
	if (overlay == NULL) {
		return;
	}
	overlay->hwdata->locked = 0;

	return;
}


int PS3_DisplayYUVOverlay(_THIS, SDL_Overlay *overlay, SDL_Rect *src, SDL_Rect *dst) {
	if ((overlay == NULL) || (overlay->hwdata == NULL)) {
		return -1;
	}

	Uint8 *lum, *Cr, *Cb;
	struct private_yuvhwdata *hwdata;
	SDL_Surface *display;

	hwdata = overlay->hwdata;
	display = hwdata->display;

	
	if ((src->w != dst->w) || (src->h != dst->h) ) {
		hwdata->scale = 1;
		deprintf(1, "[PS3] We need to scale\n");
	} else {
		hwdata->scale = 0;
		deprintf(1, "[PS3] No scaling\n");
	}

	
	switch (overlay->format) {
		case SDL_YV12_OVERLAY:
			lum = (Uint8 *)overlay->pixels[0];
			Cr =  (Uint8 *)overlay->pixels[1];
			Cb =  (Uint8 *)overlay->pixels[2];
			break;
		case SDL_IYUV_OVERLAY:
			lum = (Uint8 *)overlay->pixels[0];
			Cr =  (Uint8 *)overlay->pixels[2];
			Cb =  (Uint8 *)overlay->pixels[1];
			break;
		default:
			SDL_SetError("Unsupported YUV format in blit");
			return -1;
	}

	if (hwdata->scale) {
		
		hwdata->scaler_out = (Uint8 *) memalign(16, dst->w * dst->h + ((dst->w * dst->h) >> 1));
		if (hwdata->scaler_out == NULL) {
			SDL_FreeYUVOverlay(overlay);
			SDL_OutOfMemory();
			return -1;
		}

		
		hwdata->scaler_parms->src_pixel_width = src->w;
		hwdata->scaler_parms->src_pixel_height = src->h;
		hwdata->scaler_parms->dst_pixel_width = dst->w;
		hwdata->scaler_parms->dst_pixel_height = dst->h;
		hwdata->scaler_parms->y_plane = lum;
		hwdata->scaler_parms->v_plane = Cr;
		hwdata->scaler_parms->u_plane = Cb;
		hwdata->scaler_parms->dstBuffer = hwdata->scaler_out;
		scaler_thread_data->argp = (void *)hwdata->scaler_parms;

		
		SPE_Start(this, scaler_thread_data);
		SPE_Stop(this, scaler_thread_data);

		
		hwdata->converter_parms->y_plane = hwdata->scaler_out;
		hwdata->converter_parms->v_plane = hwdata->scaler_out + dst->w * dst->h;
		hwdata->converter_parms->u_plane = hwdata->scaler_out + dst->w * dst->h + ((dst->w * dst->h) >> 2);
	} else {
		
		hwdata->converter_parms->y_plane = lum;
		hwdata->converter_parms->v_plane = Cr;
		hwdata->converter_parms->u_plane = Cb;
	}

	hwdata->converter_parms->src_pixel_width = dst->w;
	hwdata->converter_parms->src_pixel_height = dst->h;
	hwdata->converter_parms->dstBuffer = (Uint8 *) s_pixels;
	converter_thread_data->argp = (void *)hwdata->converter_parms;

	
	SPE_SendMsg(this, converter_thread_data, SPU_START);
	SPE_SendMsg(this, converter_thread_data, (unsigned int)converter_thread_data->argp);

	
	s_bounded_input_width = dst->w;
	s_bounded_input_height = dst->h;

	
	SDL_UpdateRects(display, 1, dst);

	if (hwdata->scale)
		SDL_free((void *)hwdata->scaler_out);

	return 0;
}


void PS3_FreeYUVOverlay(_THIS, SDL_Overlay *overlay) {
	if (overlay == NULL) {
		return;
	}

	if (overlay->hwdata == NULL) {
		return;
	}

	struct private_yuvhwdata * hwdata;
	hwdata = overlay->hwdata;

	if (scaler_thread_data)
		SDL_free(scaler_thread_data);
	if (converter_thread_data) {
		SPE_Shutdown(this, converter_thread_data);
		SDL_free(converter_thread_data);
	}

	if (hwdata) {
		if (hwdata->pixels)
			SDL_free((void *)hwdata->pixels);
		SDL_free(hwdata);
	}
	return;
}

