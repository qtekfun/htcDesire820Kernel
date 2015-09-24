/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */


#ifdef __cplusplus
extern "C" {
#endif

#ifndef VPX_IMAGE_H
#define VPX_IMAGE_H

#define VPX_IMAGE_ABI_VERSION (1) 


#define VPX_IMG_FMT_PLANAR     0x100  
#define VPX_IMG_FMT_UV_FLIP    0x200  
#define VPX_IMG_FMT_HAS_ALPHA  0x400  


  
  typedef enum vpx_img_fmt {
    VPX_IMG_FMT_NONE,
    VPX_IMG_FMT_RGB24,   
    VPX_IMG_FMT_RGB32,   
    VPX_IMG_FMT_RGB565,  
    VPX_IMG_FMT_RGB555,  
    VPX_IMG_FMT_UYVY,    
    VPX_IMG_FMT_YUY2,    
    VPX_IMG_FMT_YVYU,    
    VPX_IMG_FMT_BGR24,   
    VPX_IMG_FMT_RGB32_LE, 
    VPX_IMG_FMT_ARGB,     
    VPX_IMG_FMT_ARGB_LE,  
    VPX_IMG_FMT_RGB565_LE,  
    VPX_IMG_FMT_RGB555_LE,  
    VPX_IMG_FMT_YV12    = VPX_IMG_FMT_PLANAR | VPX_IMG_FMT_UV_FLIP | 1, 
    VPX_IMG_FMT_I420    = VPX_IMG_FMT_PLANAR | 2,
    VPX_IMG_FMT_VPXYV12 = VPX_IMG_FMT_PLANAR | VPX_IMG_FMT_UV_FLIP | 3, 
    VPX_IMG_FMT_VPXI420 = VPX_IMG_FMT_PLANAR | 4,
    VPX_IMG_FMT_I422    = VPX_IMG_FMT_PLANAR | 5,
    VPX_IMG_FMT_I444    = VPX_IMG_FMT_PLANAR | 6,
    VPX_IMG_FMT_444A    = VPX_IMG_FMT_PLANAR | VPX_IMG_FMT_HAS_ALPHA | 7
  } vpx_img_fmt_t; 

#if !defined(VPX_CODEC_DISABLE_COMPAT) || !VPX_CODEC_DISABLE_COMPAT
#define IMG_FMT_PLANAR         VPX_IMG_FMT_PLANAR     
#define IMG_FMT_UV_FLIP        VPX_IMG_FMT_UV_FLIP    
#define IMG_FMT_HAS_ALPHA      VPX_IMG_FMT_HAS_ALPHA  

#define img_fmt   vpx_img_fmt
#define img_fmt_t vpx_img_fmt_t

#define IMG_FMT_NONE       VPX_IMG_FMT_NONE       
#define IMG_FMT_RGB24      VPX_IMG_FMT_RGB24      
#define IMG_FMT_RGB32      VPX_IMG_FMT_RGB32      
#define IMG_FMT_RGB565     VPX_IMG_FMT_RGB565     
#define IMG_FMT_RGB555     VPX_IMG_FMT_RGB555     
#define IMG_FMT_UYVY       VPX_IMG_FMT_UYVY       
#define IMG_FMT_YUY2       VPX_IMG_FMT_YUY2       
#define IMG_FMT_YVYU       VPX_IMG_FMT_YVYU       
#define IMG_FMT_BGR24      VPX_IMG_FMT_BGR24      
#define IMG_FMT_RGB32_LE   VPX_IMG_FMT_RGB32_LE   
#define IMG_FMT_ARGB       VPX_IMG_FMT_ARGB       
#define IMG_FMT_ARGB_LE    VPX_IMG_FMT_ARGB_LE    
#define IMG_FMT_RGB565_LE  VPX_IMG_FMT_RGB565_LE  
#define IMG_FMT_RGB555_LE  VPX_IMG_FMT_RGB555_LE  
#define IMG_FMT_YV12       VPX_IMG_FMT_YV12       
#define IMG_FMT_I420       VPX_IMG_FMT_I420       
#define IMG_FMT_VPXYV12    VPX_IMG_FMT_VPXYV12    
#define IMG_FMT_VPXI420    VPX_IMG_FMT_VPXI420    
#endif 

  
  typedef struct vpx_image {
    vpx_img_fmt_t fmt; 

    
    unsigned int  w;   
    unsigned int  h;   

    
    unsigned int  d_w;   
    unsigned int  d_h;   

    
    unsigned int  x_chroma_shift;   
    unsigned int  y_chroma_shift;   

    
#define VPX_PLANE_PACKED 0   
#define VPX_PLANE_Y      0   
#define VPX_PLANE_U      1   
#define VPX_PLANE_V      2   
#define VPX_PLANE_ALPHA  3   
#if !defined(VPX_CODEC_DISABLE_COMPAT) || !VPX_CODEC_DISABLE_COMPAT
#define PLANE_PACKED     VPX_PLANE_PACKED
#define PLANE_Y          VPX_PLANE_Y
#define PLANE_U          VPX_PLANE_U
#define PLANE_V          VPX_PLANE_V
#define PLANE_ALPHA      VPX_PLANE_ALPHA
#endif
    unsigned char *planes[4];  
    int      stride[4];  

    int     bps; 

    void    *user_priv; 

    
    unsigned char *img_data;       
    int      img_data_owner; 
    int      self_allocd;    
  } vpx_image_t; 

  
  typedef struct vpx_image_rect {
    unsigned int x; 
    unsigned int y; 
    unsigned int w; 
    unsigned int h; 
  } vpx_image_rect_t; 

  vpx_image_t *vpx_img_alloc(vpx_image_t  *img,
                             vpx_img_fmt_t fmt,
                             unsigned int d_w,
                             unsigned int d_h,
                             unsigned int align);

  vpx_image_t *vpx_img_wrap(vpx_image_t  *img,
                            vpx_img_fmt_t fmt,
                            unsigned int d_w,
                            unsigned int d_h,
                            unsigned int align,
                            unsigned char      *img_data);


  int vpx_img_set_rect(vpx_image_t  *img,
                       unsigned int  x,
                       unsigned int  y,
                       unsigned int  w,
                       unsigned int  h);


  void vpx_img_flip(vpx_image_t *img);

  void vpx_img_free(vpx_image_t *img);

#endif
#ifdef __cplusplus
}
#endif
