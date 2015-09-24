/*
 * Copyright (c) 2007 Intel Corporation. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef _VA_BACKEND_H_
#define _VA_BACKEND_H_

#include <va/va.h>
#include <linux/videodev2.h>

typedef struct VADriverContext *VADriverContextP;
typedef struct VADisplayContext *VADisplayContextP;

enum {
    
    VA_DISPLAY_MAJOR_MASK = 0xf0,

    
    VA_DISPLAY_X11      = 0x10,
    
    VA_DISPLAY_GLX      = (VA_DISPLAY_X11 | (1 << 0)),
    
    VA_DISPLAY_ANDROID  = 0x20,
    
    VA_DISPLAY_DRM      = 0x30,
    
    VA_DISPLAY_WAYLAND  = 0x40,
};

struct VADriverVTable
{
	VAStatus (*vaTerminate) ( VADriverContextP ctx );

	VAStatus (*vaQueryConfigProfiles) (
		VADriverContextP ctx,
		VAProfile *profile_list,	
		int *num_profiles			
	);

	VAStatus (*vaQueryConfigEntrypoints) (
		VADriverContextP ctx,
		VAProfile profile,
		VAEntrypoint  *entrypoint_list,	
		int *num_entrypoints			
	);

	VAStatus (*vaGetConfigAttributes) (
		VADriverContextP ctx,
		VAProfile profile,
		VAEntrypoint entrypoint,
		VAConfigAttrib *attrib_list,	
		int num_attribs
	);

	VAStatus (*vaCreateConfig) (
		VADriverContextP ctx,
		VAProfile profile, 
		VAEntrypoint entrypoint, 
		VAConfigAttrib *attrib_list,
		int num_attribs,
		VAConfigID *config_id		
	);

	VAStatus (*vaDestroyConfig) (
		VADriverContextP ctx,
		VAConfigID config_id
	);

	VAStatus (*vaQueryConfigAttributes) (
		VADriverContextP ctx,
		VAConfigID config_id, 
		VAProfile *profile,		
		VAEntrypoint *entrypoint, 	
		VAConfigAttrib *attrib_list,	
		int *num_attribs		
	);

	VAStatus (*vaCreateSurfaces) (
		VADriverContextP ctx,
		int width,
		int height,
		int format,
		int num_surfaces,
		VASurfaceID *surfaces		
	);

	VAStatus (*vaDestroySurfaces) (
		VADriverContextP ctx,
		VASurfaceID *surface_list,
		int num_surfaces
	);

	VAStatus (*vaCreateContext) (
		VADriverContextP ctx,
		VAConfigID config_id,
		int picture_width,
		int picture_height,
		int flag,
		VASurfaceID *render_targets,
		int num_render_targets,
		VAContextID *context		
	);

	VAStatus (*vaDestroyContext) (
		VADriverContextP ctx,
		VAContextID context
	);

	VAStatus (*vaCreateBuffer) (
		VADriverContextP ctx,
		VAContextID context,		
		VABufferType type,		
		unsigned int size,		
		unsigned int num_elements,	
		void *data,			
		VABufferID *buf_id		
	);

	VAStatus (*vaBufferSetNumElements) (
		VADriverContextP ctx,
		VABufferID buf_id,	
		unsigned int num_elements	
	);

	VAStatus (*vaMapBuffer) (
		VADriverContextP ctx,
		VABufferID buf_id,	
		void **pbuf         
	);

	VAStatus (*vaUnmapBuffer) (
		VADriverContextP ctx,
		VABufferID buf_id	
	);

	VAStatus (*vaDestroyBuffer) (
		VADriverContextP ctx,
		VABufferID buffer_id
	);

	VAStatus (*vaBeginPicture) (
		VADriverContextP ctx,
		VAContextID context,
		VASurfaceID render_target
	);

	VAStatus (*vaRenderPicture) (
		VADriverContextP ctx,
		VAContextID context,
		VABufferID *buffers,
		int num_buffers
	);

	VAStatus (*vaEndPicture) (
		VADriverContextP ctx,
		VAContextID context
	);

	VAStatus (*vaSyncSurface) (
		VADriverContextP ctx,
		VASurfaceID render_target
	);

	VAStatus (*vaQuerySurfaceStatus) (
		VADriverContextP ctx,
		VASurfaceID render_target,
		VASurfaceStatus *status	
	);

	VAStatus (*vaQuerySurfaceError) (
		VADriverContextP ctx,
		VASurfaceID render_target,
                VAStatus error_status,
                void **error_info 
	);

	VAStatus (*vaPutSurface) (
    		VADriverContextP ctx,
		VASurfaceID surface,
		void* draw, 
		short srcx,
		short srcy,
		unsigned short srcw,
		unsigned short srch,
		short destx,
		short desty,
		unsigned short destw,
		unsigned short desth,
		VARectangle *cliprects, 
		unsigned int number_cliprects, 
		unsigned int flags 
	);

	VAStatus (*vaQueryImageFormats) (
		VADriverContextP ctx,
		VAImageFormat *format_list,        
		int *num_formats           
	);

	VAStatus (*vaCreateImage) (
		VADriverContextP ctx,
		VAImageFormat *format,
		int width,
		int height,
		VAImage *image     
	);

	VAStatus (*vaDeriveImage) (
		VADriverContextP ctx,
		VASurfaceID surface,
		VAImage *image     
	);

	VAStatus (*vaDestroyImage) (
		VADriverContextP ctx,
		VAImageID image
	);
	
	VAStatus (*vaSetImagePalette) (
	        VADriverContextP ctx,
	        VAImageID image,
                unsigned char *palette
	);
	
	VAStatus (*vaGetImage) (
		VADriverContextP ctx,
		VASurfaceID surface,
		int x,     
		int y,
		unsigned int width, 
		unsigned int height,
		VAImageID image
	);

	VAStatus (*vaPutImage) (
		VADriverContextP ctx,
		VASurfaceID surface,
		VAImageID image,
		int src_x,
		int src_y,
		unsigned int src_width,
		unsigned int src_height,
		int dest_x,
		int dest_y,
		unsigned int dest_width,
		unsigned int dest_height
	);

	VAStatus (*vaQuerySubpictureFormats) (
		VADriverContextP ctx,
		VAImageFormat *format_list,        
		unsigned int *flags,       
		unsigned int *num_formats  
	);

	VAStatus (*vaCreateSubpicture) (
		VADriverContextP ctx,
		VAImageID image,
		VASubpictureID *subpicture   
	);

	VAStatus (*vaDestroySubpicture) (
		VADriverContextP ctx,
		VASubpictureID subpicture
	);

        VAStatus (*vaSetSubpictureImage) (
                VADriverContextP ctx,
                VASubpictureID subpicture,
                VAImageID image
        );

	VAStatus (*vaSetSubpictureChromakey) (
		VADriverContextP ctx,
		VASubpictureID subpicture,
		unsigned int chromakey_min,
		unsigned int chromakey_max,
		unsigned int chromakey_mask
	);

	VAStatus (*vaSetSubpictureGlobalAlpha) (
		VADriverContextP ctx,
		VASubpictureID subpicture,
		float global_alpha 
	);

	VAStatus (*vaAssociateSubpicture) (
		VADriverContextP ctx,
		VASubpictureID subpicture,
		VASurfaceID *target_surfaces,
		int num_surfaces,
		short src_x, 
		short src_y,
		unsigned short src_width,
		unsigned short src_height,
		short dest_x, 
		short dest_y,
		unsigned short dest_width,
		unsigned short dest_height,
		unsigned int flags
	);

	VAStatus (*vaDeassociateSubpicture) (
		VADriverContextP ctx,
		VASubpictureID subpicture,
		VASurfaceID *target_surfaces,
		int num_surfaces
	);

	VAStatus (*vaQueryDisplayAttributes) (
		VADriverContextP ctx,
		VADisplayAttribute *attr_list,	
		int *num_attributes		
        );

	VAStatus (*vaGetDisplayAttributes) (
		VADriverContextP ctx,
		VADisplayAttribute *attr_list,	
		int num_attributes
        );
        
        VAStatus (*vaSetDisplayAttributes) (
		VADriverContextP ctx,
                VADisplayAttribute *attr_list,
                int num_attributes
        );

                
        VAStatus (*vaBufferInfo) (
                   VADriverContextP ctx,      
                   VABufferID buf_id,         
                   VABufferType *type,        
                   unsigned int *size,        
                   unsigned int *num_elements 
        );

            
        VAStatus (*vaLockSurface) (
		VADriverContextP ctx,
                VASurfaceID surface,
                unsigned int *fourcc, 
                unsigned int *luma_stride,
                unsigned int *chroma_u_stride,
                unsigned int *chroma_v_stride,
                unsigned int *luma_offset,
                unsigned int *chroma_u_offset,
                unsigned int *chroma_v_offset,
                unsigned int *buffer_name, 
                void **buffer 
        );
    
        VAStatus (*vaUnlockSurface) (
		VADriverContextP ctx,
                VASurfaceID surface
        );

        
        VAStatus
        (*vaGetSurfaceAttributes)(
            VADriverContextP    dpy,
            VAConfigID          config,
            VASurfaceAttrib    *attrib_list,
            unsigned int        num_attribs
        );

        VAStatus
        (*vaCreateSurfaces2)(
            VADriverContextP    ctx,
            unsigned int        format,
            unsigned int        width,
            unsigned int        height,
            VASurfaceID        *surfaces,
            unsigned int        num_surfaces,
            VASurfaceAttrib    *attrib_list,
            unsigned int        num_attribs
        );

        VAStatus
        (*vaQuerySurfaceAttributes)(
            VADriverContextP    dpy,
            VAConfigID          config,
            VASurfaceAttrib    *attrib_list,
            unsigned int       *num_attribs
        );
};

struct VADriverContext
{
    void *pDriverData;

    struct VADriverVTable *vtable;

    struct VADriverVTableGLX *vtable_glx;

    struct VADriverVTableEGL *vtable_egl;

    void *vtable_tpi;

    void *native_dpy;
    int x11_screen;
    int version_major;
    int version_minor;
    int max_profiles;
    int max_entrypoints;
    int max_attributes;
    int max_image_formats;
    int max_subpic_formats;
    int max_display_attributes;
    const char *str_vendor;

    void *handle;			

    void *drm_state;

    void *glx;				

    
    unsigned long display_type;

    struct VADriverVTableWayland *vtable_wayland;

    struct VADriverVTableVPP *vtable_vpp;

    unsigned long reserved[42];         
};

#define VA_DISPLAY_MAGIC 0x56414430 
struct VADisplayContext
{
    int vadpy_magic;
    
    VADisplayContextP pNext;
    VADriverContextP pDriverContext;

    int (*vaIsValid) (
	VADisplayContextP ctx
    );

    void (*vaDestroy) (
	VADisplayContextP ctx
    );

    VAStatus (*vaGetDriverName) (
	VADisplayContextP ctx,
	char **driver_name
    );

    void *opaque; 
};

typedef VAStatus (*VADriverInit) (
    VADriverContextP driver_context
);

#endif 
