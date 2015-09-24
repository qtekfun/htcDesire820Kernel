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


#ifndef _SDL_ATARIGL_H_
#define _SDL_ATARIGL_H_

#if SDL_VIDEO_OPENGL
#include <GL/osmesa.h>
#endif

#include "../SDL_sysvideo.h"

#define _THIS   SDL_VideoDevice *this

struct SDL_PrivateGLData {

    int gl_active; 	
	
	int gl_oldmesa;	

	int	gl_pixelsize;	

	SDL_bool gl_upsidedown; 

	Uint8 *gl_shadow;	

	
	void (*ConvertSurface)(_THIS, SDL_Surface *surface);	

	
	void (*CopyShadow)(_THIS, SDL_Surface *surface);	

#if SDL_VIDEO_OPENGL
	OSMesaContext	ctx;

	
	void (*glGetIntegerv)( GLenum pname, GLint *value );
	void (*glFinish)(void);
	void (*glFlush)(void);

	
	OSMesaContext (*OSMesaCreateContextExt)( GLenum format, GLint depthBits, GLint stencilBits, GLint accumBits, OSMesaContext sharelist);
	void (*OSMesaDestroyContext)( OSMesaContext ctx );
	GLboolean (*OSMesaMakeCurrent)( OSMesaContext ctx, void *buffer, GLenum type, GLsizei width, GLsizei height );
	void (*OSMesaPixelStore)( GLint pname, GLint value );
	void * (*OSMesaGetProcAddress)( const char *funcName );

	
	void *(*OSMesaCreateLDG)( long format, long type, long width, long height );
	void (*OSMesaDestroyLDG)(void);

	
	int width, height;
	GLenum format;
	GLint depth,stencil,accum;
#endif
};

#define gl_active		(this->gl_data->gl_active)
#define gl_ctx			(this->gl_data->ctx)
#define gl_oldmesa		(this->gl_data->gl_oldmesa)
#define gl_pixelsize	(this->gl_data->gl_pixelsize)
#define gl_upsidedown	(this->gl_data->gl_upsidedown)
#define gl_shadow		(this->gl_data->gl_shadow)
#define gl_convert		(this->gl_data->ConvertSurface)
#define gl_copyshadow	(this->gl_data->CopyShadow)
#define gl_curformat	(this->gl_data->format)
#define gl_curdepth		(this->gl_data->depth)
#define gl_curstencil	(this->gl_data->stencil)
#define gl_curaccum		(this->gl_data->accum)
#define gl_curwidth		(this->gl_data->width)
#define gl_curheight	(this->gl_data->height)

extern int SDL_AtariGL_Init(_THIS, SDL_Surface *current);
extern void SDL_AtariGL_Quit(_THIS, SDL_bool unload);
extern void SDL_AtariGL_InitPointers(_THIS);

extern int SDL_AtariGL_LoadLibrary(_THIS, const char *path);
extern void *SDL_AtariGL_GetProcAddress(_THIS, const char *proc);
extern int SDL_AtariGL_GetAttribute(_THIS, SDL_GLattr attrib, int* value);
extern int SDL_AtariGL_MakeCurrent(_THIS);
extern void SDL_AtariGL_SwapBuffers(_THIS);

#endif 
