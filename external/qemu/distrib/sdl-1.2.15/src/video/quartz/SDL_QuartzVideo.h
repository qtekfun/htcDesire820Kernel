/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2012  Sam Lantinga

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


#include "SDL_opengl.h"

#include <Cocoa/Cocoa.h>
#include <Carbon/Carbon.h>
#include <OpenGL/OpenGL.h>	
#include <IOKit/IOKitLib.h>	
#include <pthread.h>

#include "SDL_thread.h"
#include "SDL_video.h"
#include "SDL_error.h"
#include "SDL_timer.h"
#include "SDL_loadso.h"
#include "SDL_syswm.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"


#ifdef __powerpc__
@interface NSOpenGLContext (CGLContextAccess)
- (CGLContextObj) cglContext;
@end
#endif

CGLContextObj QZ_GetCGLContextObj(NSOpenGLContext *nsctx);


typedef struct SDL_PrivateVideoData {
    BOOL               use_new_mode_apis;  
    BOOL               allow_screensaver;  
    CGDirectDisplayID  display;            
    const void         *mode;              
    const void         *save_mode;         
    CGDirectPaletteRef palette;            
    NSOpenGLContext    *gl_context;        
    NSGraphicsContext  *nsgfx_context;     
    Uint32             width, height, bpp; 
    Uint32             flags;              
    Uint32             video_set;          
    Uint32             warp_flag;          
    Uint32             warp_ticks;         
    NSWindow           *window;            
    NSView             *view;              
    CGContextRef       cg_context;         
    SDL_Surface        *resize_icon;       
    SDL_GrabMode       current_grab_mode;  
    SDL_Rect           **client_mode_list; 
    SDLKey             keymap[256];        
    Uint32             current_mods;       
    NSText             *field_edit;        
    Uint32             last_virtual_button;
    io_connect_t       power_connection;   
    Uint8              expect_mouse_up;    
    Uint8              grab_state;         
    NSPoint            cursor_loc;         
    BOOL               cursor_should_be_visible;     
    BOOL               cursor_visible;     
    Uint8*             sw_buffers[2];      
    SDL_Thread         *thread;            
    SDL_sem            *sem1, *sem2;       
    Uint8              *current_buffer;    
    BOOL               quit_thread;        
    SInt32             system_version;     

    void *opengl_library;    
} SDL_PrivateVideoData;

#define _THIS    SDL_VideoDevice *this
#define display_id (this->hidden->display)
#define mode (this->hidden->mode)
#define save_mode (this->hidden->save_mode)
#define use_new_mode_apis (this->hidden->use_new_mode_apis)
#define allow_screensaver (this->hidden->allow_screensaver)
#define palette (this->hidden->palette)
#define gl_context (this->hidden->gl_context)
#define nsgfx_context (this->hidden->nsgfx_context)
#define device_width (this->hidden->width)
#define device_height (this->hidden->height)
#define device_bpp (this->hidden->bpp)
#define mode_flags (this->hidden->flags)
#define qz_window (this->hidden->window)
#define window_view (this->hidden->view)
#define cg_context (this->hidden->cg_context)
#define video_set (this->hidden->video_set)
#define warp_ticks (this->hidden->warp_ticks)
#define warp_flag (this->hidden->warp_flag)
#define resize_icon (this->hidden->resize_icon)
#define current_grab_mode (this->hidden->current_grab_mode)
#define client_mode_list (this->hidden->client_mode_list)
#define keymap (this->hidden->keymap)
#define current_mods (this->hidden->current_mods)
#define field_edit (this->hidden->field_edit)
#define last_virtual_button (this->hidden->last_virtual_button)
#define power_connection (this->hidden->power_connection)
#define expect_mouse_up (this->hidden->expect_mouse_up)
#define grab_state (this->hidden->grab_state)
#define cursor_loc (this->hidden->cursor_loc)
#define cursor_should_be_visible (this->hidden->cursor_should_be_visible)
#define cursor_visible (this->hidden->cursor_visible)
#define sw_buffers (this->hidden->sw_buffers)
#define sw_contexts (this->hidden->sw_contexts)
#define thread (this->hidden->thread)
#define sem1 (this->hidden->sem1)
#define sem2 (this->hidden->sem2)
#define current_buffer (this->hidden->current_buffer)
#define quit_thread (this->hidden->quit_thread)
#define system_version (this->hidden->system_version)
#define opengl_library (this->hidden->opengl_library)

enum {
    QZ_UNGRABBED = 0,
    QZ_VISIBLE_GRAB,
    QZ_INVISIBLE_GRAB
};

enum {
    QZ_ENABLE_GRAB = 0,
    QZ_DISABLE_GRAB,
    QZ_HIDECURSOR,
    QZ_SHOWCURSOR
};

int    QZ_SetGamma          (_THIS, float red, float green, float blue);
int    QZ_GetGamma          (_THIS, float *red, float *green, float *blue);
int    QZ_SetGammaRamp      (_THIS, Uint16 *ramp);
int    QZ_GetGammaRamp      (_THIS, Uint16 *ramp);

int    QZ_SetupOpenGL       (_THIS, int bpp, Uint32 flags);
void   QZ_TearDownOpenGL    (_THIS);
void*  QZ_GL_GetProcAddress (_THIS, const char *proc);
int    QZ_GL_GetAttribute   (_THIS, SDL_GLattr attrib, int* value);
int    QZ_GL_MakeCurrent    (_THIS);
void   QZ_GL_SwapBuffers    (_THIS);
int    QZ_GL_LoadLibrary    (_THIS, const char *location);

void         QZ_FreeWMCursor     (_THIS, WMcursor *cursor);
WMcursor*    QZ_CreateWMCursor   (_THIS, Uint8 *data, Uint8 *mask,
                                  int w, int h, int hot_x, int hot_y);
int          QZ_ShowWMCursor     (_THIS, WMcursor *cursor);
void         QZ_WarpWMCursor     (_THIS, Uint16 x, Uint16 y);
void         QZ_MoveWMCursor     (_THIS, int x, int y);
void         QZ_CheckMouseMode   (_THIS);
void         QZ_UpdateMouse      (_THIS);

void         QZ_InitOSKeymap     (_THIS);
void         QZ_PumpEvents       (_THIS);

void         QZ_SetCaption       (_THIS, const char *title, const char *icon);
void         QZ_SetIcon          (_THIS, SDL_Surface *icon, Uint8 *mask);
int          QZ_IconifyWindow    (_THIS);
void         QZ_SetWindowPos     (_THIS, int x, int y);
void         QZ_GetWindowPos     (_THIS, int *px, int *py);
int          QZ_IsWindowVisible  (_THIS, int recenter);
int          QZ_GetMonitorDPI    (_THIS, int *xDpi, int *yDpi);
int          QZ_GetMonitorRect   (_THIS, SDL_Rect *rect);

SDL_GrabMode QZ_GrabInput        (_THIS, SDL_GrabMode grab_mode);

void         QZ_PrivateWarpCursor (_THIS, int x, int y);
void         QZ_ChangeGrabState (_THIS, int action);
void         QZ_RegisterForSleepNotifications (_THIS);
void         QZ_PrivateGlobalToLocal (_THIS, NSPoint *p);
void         QZ_PrivateCocoaToSDL (_THIS, NSPoint *p);
BOOL         QZ_IsMouseInWindow (_THIS);
void         QZ_DoActivate (_THIS);
void         QZ_DoDeactivate (_THIS);
