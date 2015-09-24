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
#include "../SDL_sysvideo.h"
#include "SDL_ps3events_c.h"
#include "SDL_ps3video.h"
#include "SDL_ps3yuv_c.h"
#include "spulibs/spu_common.h"

#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
#include <sys/mman.h>

#include <linux/fb.h>
#include <asm/ps3fb.h>
#include <libspe2.h>
#include <malloc.h>

static int PS3_Available();
static SDL_VideoDevice *PS3_CreateDevice(int devindex);
static int PS3_VideoInit(_THIS, SDL_PixelFormat * vformat);
static void PS3_VideoQuit(_THIS);
static void PS3_DeleteDevice(SDL_VideoDevice * device);
static SDL_Surface *PS3_SetVideoMode(_THIS, SDL_Surface * current, int width, int height, int bpp, Uint32 flags);
static SDL_Rect **PS3_ListModes(_THIS, SDL_PixelFormat * format, Uint32 flags);

static int PS3_AllocHWSurface(_THIS, SDL_Surface * surface);
static void PS3_FreeHWSurface(_THIS, SDL_Surface * surface);
static int PS3_LockHWSurface(_THIS, SDL_Surface * surface);
static void PS3_UnlockHWSurface(_THIS, SDL_Surface * surface);
static int PS3_FlipDoubleBuffer(_THIS, SDL_Surface * surface);
static void PS3_DoubleBufferUpdate(_THIS, int numrects, SDL_Rect * rects);

int SPE_Start(_THIS, spu_data_t * spe_data);
int SPE_Stop(_THIS, spu_data_t * spe_data);
int SPE_Boot(_THIS, spu_data_t * spe_data);
int SPE_Shutdown(_THIS, spu_data_t * spe_data);
int SPE_SendMsg(_THIS, spu_data_t * spe_data, unsigned int msg);
int SPE_WaitForMsg(_THIS, spu_data_t * spe_data, unsigned int msg);
void SPE_RunContext(void *thread_argp);

void enable_cursor(int enable);

extern spe_program_handle_t fb_writer_spu;

static int PS3_Available()
{
	return 1;
}

static SDL_VideoDevice *PS3_CreateDevice(int devindex)
{
	SDL_VideoDevice *this;

	
	this = (SDL_VideoDevice *) SDL_malloc(sizeof(SDL_VideoDevice));
	if (this) {
		memset(this, 0, sizeof *this);
		this->hidden = (struct SDL_PrivateVideoData *)
		    SDL_malloc(sizeof(struct SDL_PrivateVideoData));
	}
	
	if ((this == NULL) || (this->hidden == NULL)) {
		SDL_OutOfMemory();
		if (this)
			SDL_free(this);
		return 0;
	}
	memset(this->hidden, 0, sizeof(struct SDL_PrivateVideoData));

	
	this->VideoInit = PS3_VideoInit;
	this->ListModes = PS3_ListModes;
	this->SetVideoMode = PS3_SetVideoMode;
	this->SetColors = 0;
	this->CreateYUVOverlay = PS3_CreateYUVOverlay;
	this->UpdateRects = 0;
	this->VideoQuit = PS3_VideoQuit;
	this->AllocHWSurface = PS3_AllocHWSurface;
	this->CheckHWBlit = 0;
	this->FillHWRect = 0;
	this->SetHWColorKey = 0;
	this->SetHWAlpha = 0;
	this->LockHWSurface = PS3_LockHWSurface;
	this->UnlockHWSurface = PS3_UnlockHWSurface;
	this->FlipHWSurface = PS3_FlipDoubleBuffer;
	this->FreeHWSurface = PS3_FreeHWSurface;
	this->SetCaption = 0;
	this->SetIcon = 0;
	this->IconifyWindow = 0;
	this->GrabInput = 0;
	this->GetWMInfo = 0;
	this->InitOSKeymap = PS3_InitOSKeymap;
	this->PumpEvents = PS3_PumpEvents;

	this->free = PS3_DeleteDevice;

	return this;
}


VideoBootStrap PS3_bootstrap = {
	"ps3", "PS3 Cell SPU Driver",
	PS3_Available, PS3_CreateDevice
};


static void PS3_DeleteDevice(SDL_VideoDevice * device)
{
	free(device->hidden);
	free(device);
}


static int PS3_VideoInit(_THIS, SDL_PixelFormat * vformat)
{
	
	enable_cursor(0);

	
	fb_parms = (struct fb_writer_parms_t *)
	    memalign(16, sizeof(struct fb_writer_parms_t));
	fb_thread_data = (spu_data_t *) malloc(sizeof(spu_data_t));
	if (fb_parms == NULL || fb_thread_data == NULL) {
		SDL_OutOfMemory();
		return -1;
	}
	fb_thread_data->program = fb_writer_spu;
	fb_thread_data->program_name = "fb_writer_spu";
	fb_thread_data->argp = (void *)fb_parms;
	fb_thread_data->keepalive = 1;
	fb_thread_data->booted = 0;

	SPE_Start(this, fb_thread_data);

	
	fb_dev_fd = open(PS3_DEV_FB, O_RDWR);
	if (fb_dev_fd < 0) {
		SDL_SetError("[PS3] Unable to open device %s", PS3_DEV_FB);
		return -1;
	}

	
	if (ioctl(fb_dev_fd, FBIOGET_VSCREENINFO, &fb_vinfo)) {
		SDL_SetError("[PS3] Can't get VSCREENINFO");
		if (fb_dev_fd >= 0)
			close(fb_dev_fd);
		fb_dev_fd = -1;
		return -1;
	}

	
	this->info.current_w = fb_vinfo.xres;
	this->info.current_h = fb_vinfo.yres;
	this->info.wm_available = 0;
	this->info.hw_available = 1;

	
	fb_orig_vinfo = fb_vinfo;

	
	fb_bits_per_pixel = fb_vinfo.bits_per_pixel;
	if (fb_bits_per_pixel == 16)
		fb_bits_per_pixel =
		    fb_vinfo.red.length + fb_vinfo.green.length +
		    fb_vinfo.blue.length;

	
	vformat->BitsPerPixel = fb_vinfo.bits_per_pixel;

	fb_vinfo.xres_virtual = fb_vinfo.xres;
	fb_vinfo.yres_virtual = fb_vinfo.yres;

	
	if (ioctl(fb_dev_fd, FBIOPUT_VSCREENINFO, &fb_vinfo)) {
		SDL_SetError("[PS3] Can't put VSCREENINFO");
		if (fb_dev_fd >= 0)
			close(fb_dev_fd);
		fb_dev_fd = -1;
		return -1;
	}

	s_fb_pixel_size = fb_vinfo.bits_per_pixel / 8;

	s_writeable_width = fb_vinfo.xres;
	s_writeable_height = fb_vinfo.yres;

	
	if (ioctl(fb_dev_fd, PS3FB_IOCTL_SCREENINFO, (unsigned long)&res) < 0) {
		SDL_SetError("[PS3] PS3FB_IOCTL_SCREENINFO failed");
	}
	deprintf(1, "[PS3] xres:%d yres:%d xoff:%d yoff:%d\n", res.xres, res.yres, res.xoff, res.yoff);

	
	if (res.num_frames < 2) {
		double_buffering = 0;
	} else {
		double_buffering = 1;
	}

	real_width = res.xres;
	real_height = res.yres;

	ioctl(fb_dev_fd, PS3FB_IOCTL_ON, 0);

	
	ioctl(fb_dev_fd, FBIOBLANK, 0);

	return 0;
}


static SDL_Rect **PS3_ListModes(_THIS, SDL_PixelFormat * format, Uint32 flags)
{
	static SDL_Rect PS3_resolutions[] = {
		{0, 0, 1920, 1080}, 
		{0, 0, 1600, 1200}, 
		{0, 0, 1280, 1024}, 
		{0, 0, 1280, 720},  
		{0, 0, 1024, 768},  
		{0, 0, 1024, 576},  
		{0, 0, 853, 480},   
		{0, 0, 720, 576},   
		{0, 0, 720, 480},   
	};
	static SDL_Rect *PS3_modes[] = {
		&PS3_resolutions[0],
		&PS3_resolutions[1],
		&PS3_resolutions[2],
		&PS3_resolutions[3],
		&PS3_resolutions[4],
		&PS3_resolutions[5],
		&PS3_resolutions[6],
		&PS3_resolutions[7],
		&PS3_resolutions[8],
		NULL
	};
	SDL_Rect **modes = PS3_modes;

	return modes;
}


static SDL_Surface *PS3_SetVideoMode(_THIS, SDL_Surface * current, int width, int height, int bpp, Uint32 flags)
{
	s_bounded_input_width = width < s_writeable_width ? width : s_writeable_width;
	s_bounded_input_height = height < s_writeable_height ? height : s_writeable_height;
	s_bounded_input_width_offset = (s_writeable_width - s_bounded_input_width) >> 1;
	s_bounded_input_height_offset = (s_writeable_height - s_bounded_input_height) >> 1;
	s_input_line_length = width * s_fb_pixel_size;

	current->flags |= flags;

	if (ioctl(fb_dev_fd, FBIOGET_FSCREENINFO, &fb_finfo)) {
		SDL_SetError("[PS3] Can't get fixed screeninfo");
		return NULL;
	}

	if (fb_finfo.type != FB_TYPE_PACKED_PIXELS) {
		SDL_SetError("[PS3] type %s not supported",
			     fb_finfo.type);
		return NULL;
	}

	
	if ((frame_buffer =
	     (uint8_t *) mmap(0, fb_finfo.smem_len,
			      PROT_READ | PROT_WRITE, MAP_SHARED,
			      fb_dev_fd, 0)) == (uint8_t *) - 1) {
		SDL_SetError("[PS3] Can't mmap for %s", PS3_DEV_FB);
		return NULL;
	} else {
		current->flags |= SDL_DOUBLEBUF;
	}
	if (!SDL_ReallocFormat(current, fb_bits_per_pixel, 0, 0, 0, 0)) {
		return (NULL);
	}

	
	memset(frame_buffer, 0x00, fb_finfo.smem_len);

	
	s_center[0] =
	    frame_buffer + s_bounded_input_width_offset * s_fb_pixel_size +
	    s_bounded_input_height_offset * fb_finfo.line_length;
	s_center[1] = s_center[0] + real_height * fb_finfo.line_length;
	s_center_index = 0;

	current->flags |= SDL_FULLSCREEN;
	current->w = width;
	current->h = height;
	current->pitch = SDL_CalculatePitch(current);

	
	s_pixels = memalign(16, current->h * current->pitch);
	current->pixels = (void *)s_pixels;
	if (!current->pixels) {
		SDL_OutOfMemory();
		return NULL;
	}

	
	this->UpdateRects = PS3_DoubleBufferUpdate;

	return current;
}


void PS3_DoubleBufferUpdate(_THIS, int numrects, SDL_Rect * rects)
{
	if (converter_thread_data && converter_thread_data->booted)
		SPE_WaitForMsg(this, converter_thread_data, SPU_FIN);

	
	s_bounded_input_width_offset = (s_writeable_width - s_bounded_input_width) >> 1;
	s_bounded_input_height_offset = (s_writeable_height - s_bounded_input_height) >> 1;
	s_center[0] = frame_buffer + s_bounded_input_width_offset * s_fb_pixel_size +
		s_bounded_input_height_offset * fb_finfo.line_length;
	s_center[1] = s_center[0] + real_height * fb_finfo.line_length;

	
	fb_parms->data = (unsigned char *)s_pixels;
	fb_parms->center = s_center[s_center_index];
	fb_parms->out_line_stride = fb_finfo.line_length;
	fb_parms->in_line_stride = s_input_line_length;
	fb_parms->bounded_input_height = s_bounded_input_height;
	fb_parms->bounded_input_width = s_bounded_input_width;
	fb_parms->fb_pixel_size = s_fb_pixel_size;

	deprintf(3, "[PS3->SPU] fb_thread_data->argp = 0x%x\n", fb_thread_data->argp);

	
	SPE_SendMsg(this, fb_thread_data, SPU_START);
	SPE_SendMsg(this, fb_thread_data, (unsigned int)fb_thread_data->argp);

	SPE_WaitForMsg(this, fb_thread_data, SPU_FIN);

	
	if (double_buffering)
		s_center_index = s_center_index ^ 0x01;
	PS3_FlipDoubleBuffer(this, this->screen);
}


void enable_cursor(int enable)
{
	int fd = open("/dev/console", O_RDWR | O_NONBLOCK);
	if (fd >= 0) {
		ioctl(fd, KDSETMODE, enable ? KD_TEXT : KD_GRAPHICS);
		close(fd);
	}
}


static int PS3_AllocHWSurface(_THIS, SDL_Surface * surface)
{
	return -1;
}


static void PS3_FreeHWSurface(_THIS, SDL_Surface * surface)
{
	return;
}


static int PS3_LockHWSurface(_THIS, SDL_Surface * surface)
{
	return 0;
}


static void PS3_UnlockHWSurface(_THIS, SDL_Surface * surface)
{
	return;
}


int PS3_FlipDoubleBuffer(_THIS, SDL_Surface * surface)
{
	unsigned long crt = 0;
	
	deprintf(1, "[PS3] Wait for vsync\n");
	ioctl(fb_dev_fd, FBIO_WAITFORVSYNC, &crt);
	
	deprintf(1, "[PS3] Page flip to buffer #%u 0x%x\n", s_center_index, s_center[s_center_index]);
	ioctl(fb_dev_fd, PS3FB_IOCTL_FSEL, (unsigned long)&s_center_index);
	return 1;
}


int SPE_Start(_THIS, spu_data_t * spe_data)
{
	deprintf(2, "[PS3->SPU] Start SPE: %s\n", spe_data->program_name);
	if (!(spe_data->booted))
		SPE_Boot(this, spe_data);

	
	spe_data->entry = SPE_DEFAULT_ENTRY;
	spe_data->error_code = 0;

	
	deprintf(2, "[PS3->SPU] Create Thread: %s\n", spe_data->program_name);
	if (pthread_create
	    (&spe_data->thread, NULL, (void *)&SPE_RunContext, (void *)spe_data)) {
		deprintf(2, "[PS3->SPU] Could not create pthread for spe: %s\n", spe_data->program_name);
		SDL_SetError("[PS3->SPU] Could not create pthread for spe");
		return -1;
	}

	if (spe_data->keepalive)
		SPE_WaitForMsg(this, spe_data, SPU_READY);
}


int SPE_Stop(_THIS, spu_data_t * spe_data)
{
	deprintf(2, "[PS3->SPU] Stop SPE: %s\n", spe_data->program_name);
	
	deprintf(2, "[PS3->SPU] Wait for SPE thread to complete: %s\n", spe_data->program_name);
	if (pthread_join(spe_data->thread, NULL)) {
		deprintf(2, "[PS3->SPU] Failed joining the thread: %s\n", spe_data->program_name);
		SDL_SetError("[PS3->SPU] Failed joining the thread");
		return -1;
	}

	return 0;
}


int SPE_Boot(_THIS, spu_data_t * spe_data)
{
	
	deprintf(2, "[PS3->SPU] Create SPE Context: %s\n", spe_data->program_name);
	spe_data->ctx = spe_context_create(0, NULL);
	if (spe_data->ctx == NULL) {
		deprintf(2, "[PS3->SPU] Failed creating SPE context: %s\n", spe_data->program_name);
		SDL_SetError("[PS3->SPU] Failed creating SPE context");
		return -1;
	}

	
	deprintf(2, "[PS3->SPU] Load Program into SPE: %s\n", spe_data->program_name);
	if (spe_program_load(spe_data->ctx, &spe_data->program)) {
		deprintf(2, "[PS3->SPU] Failed loading program into SPE context: %s\n", spe_data->program_name);
		SDL_SetError
		    ("[PS3->SPU] Failed loading program into SPE context");
		return -1;
	}
	spe_data->booted = 1;
	deprintf(2, "[PS3->SPU] SPE boot successful\n");

	return 0;
}

int SPE_Shutdown(_THIS, spu_data_t * spe_data)
{
	if (spe_data->keepalive && spe_data->booted) {
		SPE_SendMsg(this, spe_data, SPU_EXIT);
		SPE_Stop(this, spe_data);
	}

	
	deprintf(2, "[PS3->SPU] Destroy SPE context: %s\n", spe_data->program_name);
	if (spe_context_destroy(spe_data->ctx)) {
		deprintf(2, "[PS3->SPU] Failed destroying context: %s\n", spe_data->program_name);
		SDL_SetError("[PS3->SPU] Failed destroying context");
		return -1;
	}
	deprintf(2, "[PS3->SPU] SPE shutdown successful: %s\n", spe_data->program_name);
	return 0;
}


int SPE_SendMsg(_THIS, spu_data_t * spe_data, unsigned int msg)
{
	deprintf(2, "[PS3->SPU] Sending message %u to %s\n", msg, spe_data->program_name);
	
	unsigned int spe_in_mbox_msgs[1];
	spe_in_mbox_msgs[0] = msg;
	int in_mbox_write = spe_in_mbox_write(spe_data->ctx, spe_in_mbox_msgs, 1, SPE_MBOX_ALL_BLOCKING);

	if (1 > in_mbox_write) {
		deprintf(2, "[PS3->SPU] No message could be written to %s\n", spe_data->program_name);
		SDL_SetError("[PS3->SPU] No message could be written");
		return -1;
	}
	return 0;
}


int SPE_WaitForMsg(_THIS, spu_data_t * spe_data, unsigned int msg)
{
	deprintf(2, "[PS3->SPU] Waiting for message from %s\n", spe_data->program_name);
	unsigned int out_messages[1];
	while (!spe_out_mbox_status(spe_data->ctx));
	int mbox_read = spe_out_mbox_read(spe_data->ctx, out_messages, 1);
	deprintf(2, "[PS3->SPU] Got message from %s, message was %u\n", spe_data->program_name, out_messages[0]);
	if (out_messages[0] == msg)
		return 0;
	else
		return -1;
}


void SPE_RunContext(void *thread_argp)
{
	
	spu_data_t *args = (spu_data_t *) thread_argp;
	deprintf(3, "[PS3->SPU] void* argp=0x%x\n", (unsigned int)args->argp);

	
	deprintf(2, "[PS3->SPU] Run SPE program: %s\n", args->program_name);
	if (spe_context_run
	    (args->ctx, &args->entry, 0, (void *)args->argp, NULL,
	     NULL) < 0) {
		deprintf(2, "[PS3->SPU] Failed running SPE context: %s\n", args->program_name);
		SDL_SetError("[PS3->SPU] Failed running SPE context: %s", args->program_name);
		exit(1);
	}

	pthread_exit(NULL);
}


static void PS3_VideoQuit(_THIS)
{
	if (fb_dev_fd > 0) {
		
		if (ioctl(fb_dev_fd, FBIOPUT_VSCREENINFO, &fb_orig_vinfo))
			SDL_SetError("[PS3] Can't restore original fb_var_screeninfo");

		
		ioctl(fb_dev_fd, PS3FB_IOCTL_OFF, 0);
		close(fb_dev_fd);
		fb_dev_fd = -1;
	}

	if (frame_buffer) {
		munmap(frame_buffer, fb_finfo.smem_len);
		frame_buffer = 0;
	}

	if (fb_parms)
		free((void *)fb_parms);
	if (fb_thread_data) {
		SPE_Shutdown(this, fb_thread_data);
		free((void *)fb_thread_data);
	}

	if (this->screen) {
		if (double_buffering && this->screen->pixels) {
			free(this->screen->pixels);
		}
		this->screen->pixels = NULL;
	}

	enable_cursor(1);
	deprintf(1, "[PS3] VideoQuit\n");
}

