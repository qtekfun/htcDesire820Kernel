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
#include "../SDL_sysvideo.h"
#include "SDL_mouse.h"
#include "SDL_mutex.h"
#include "spulibs/spu_common.h"

#include <libspe2.h>
#include <pthread.h>
#include <linux/types.h>
#include <linux/fb.h>
#include <asm/ps3fb.h>
#include <linux/vt.h>
#include <termios.h>

#ifndef _SDL_ps3video_h
#define _SDL_ps3video_h

#define DEBUG_LEVEL 0

#ifdef DEBUG_LEVEL
#define deprintf( level, fmt, args... ) \
    do \
{ \
    if ( (unsigned)(level) <= DEBUG_LEVEL ) \
    { \
        fprintf( stdout, fmt, ##args ); \
        fflush( stdout ); \
    } \
} while ( 0 )
#else
#define deprintf( level, fmt, args... )
#endif

#define PS3_DEV_FB "/dev/fb0"

#define _THIS   SDL_VideoDevice * this

typedef struct spu_data {
    spe_context_ptr_t ctx;
    pthread_t thread;
    spe_program_handle_t program;
    char * program_name;
    unsigned int booted;
    unsigned int keepalive;
    unsigned int entry;
    int error_code;
    void * argp;
} spu_data_t;

struct SDL_PrivateVideoData
{
    const char * const fb_dev_name; 
    int fb_dev_fd; 
    uint8_t * frame_buffer; 

    
    spu_data_t * fb_thread_data;
    spu_data_t * scaler_thread_data;
    spu_data_t * converter_thread_data;

    
    struct fb_fix_screeninfo fb_finfo;
    struct fb_var_screeninfo fb_vinfo;
    struct fb_var_screeninfo fb_orig_vinfo;

    
    struct ps3fb_ioctl_res res;

    unsigned int double_buffering;
    uint32_t real_width;      
    uint32_t real_height;     

    uint32_t s_fb_pixel_size;   
    uint32_t fb_bits_per_pixel;   

    uint32_t config_count;

    uint32_t s_input_line_length;   
    uint32_t s_bounded_input_width; 
    uint32_t s_bounded_input_height;
    uint32_t s_bounded_input_width_offset;  
    uint32_t s_bounded_input_height_offset; 
    uint32_t s_writeable_width; 
    uint32_t s_writeable_height;    

    uint8_t * s_center[2]; 
    uint32_t s_center_index;

    volatile void * s_pixels __attribute__((aligned(128)));

    
    volatile struct fb_writer_parms_t * fb_parms __attribute__((aligned(128)));
};

#define fb_dev_name     (this->hidden->fb_dev_name)
#define fb_dev_fd       (this->hidden->fb_dev_fd)
#define frame_buffer       (this->hidden->frame_buffer)
#define fb_thread_data      (this->hidden->fb_thread_data)
#define scaler_thread_data      (this->hidden->scaler_thread_data)
#define converter_thread_data      (this->hidden->converter_thread_data)
#define fb_parms           (this->hidden->fb_parms)
#define SDL_nummodes		(this->hidden->SDL_nummodes)
#define SDL_modelist		(this->hidden->SDL_modelist)
#define SDL_videomode		(this->hidden->SDL_videomode)
#define fb_finfo        (this->hidden->fb_finfo)
#define fb_vinfo        (this->hidden->fb_vinfo)
#define fb_orig_vinfo   (this->hidden->fb_orig_vinfo)
#define res             (this->hidden->res)
#define double_buffering (this->hidden->double_buffering)
#define real_width      (this->hidden->real_width)
#define real_height     (this->hidden->real_height)
#define s_fb_pixel_size   (this->hidden->s_fb_pixel_size)
#define fb_bits_per_pixel (this->hidden->fb_bits_per_pixel)
#define config_count (this->hidden->config_count)
#define s_input_line_length (this->hidden->s_input_line_length)
#define s_bounded_input_width (this->hidden->s_bounded_input_width)
#define s_bounded_input_height (this->hidden->s_bounded_input_height)
#define s_bounded_input_width_offset (this->hidden->s_bounded_input_width_offset)
#define s_bounded_input_height_offset (this->hidden->s_bounded_input_height_offset)
#define s_writeable_width (this->hidden->s_writeable_width)
#define s_writeable_height (this->hidden->s_writeable_height)
#define s_center          (this->hidden->s_center)
#define s_center_index    (this->hidden->s_center_index)
#define s_pixels           (this->hidden->s_pixels)

#endif 


