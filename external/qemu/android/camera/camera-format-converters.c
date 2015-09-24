/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#elif _DARWIN_C_SOURCE
#else
#include <linux/videodev2.h>
#endif
#include "android/camera/camera-format-converters.h"

#define  E(...)    derror(__VA_ARGS__)
#define  W(...)    dwarning(__VA_ARGS__)
#define  D(...)    VERBOSE_PRINT(camera,__VA_ARGS__)
#define  D_ACTIVE  VERBOSE_CHECK(camera)



#ifndef HOST_WORDS_BIGENDIAN
static const uint16_t kRed5     = 0x001f;
static const uint16_t kGreen6   = 0x07e0;
static const uint16_t kBlue5    = 0xf800;
#else   
static const uint16_t kRed5     = 0xf800;
static const uint16_t kGreen6   = 0x07e0;
static const uint16_t kBlue5    = 0x001f;
#endif  


#ifndef HOST_WORDS_BIGENDIAN
static const uint32_t kRed8     = 0x000000ff;
static const uint32_t kGreen8   = 0x0000ff00;
static const uint32_t kBlue8    = 0x00ff0000;
#else   
static const uint32_t kRed8     = 0x00ff0000;
static const uint32_t kGreen8   = 0x0000ff00;
static const uint32_t kBlue8    = 0x000000ff;
#endif  


#ifndef HOST_WORDS_BIGENDIAN
#define R16(rgb)    (uint8_t)((rgb) & kRed5)
#define G16(rgb)    (uint8_t)(((rgb) & kGreen6) >> 5)
#define B16(rgb)    (uint8_t)(((rgb) & kBlue5) >> 11)
#define R16_32(rgb) (uint8_t)((((rgb) & kRed5) << 3) | (((rgb) & kRed5) >> 2))
#define G16_32(rgb) (uint8_t)((((rgb) & kGreen6) >> 3) | (((rgb) & kGreen6) >> 9))
#define B16_32(rgb) (uint8_t)((((rgb) & kBlue5) >> 8) | (((rgb) & kBlue5) >> 14))
#define R32(rgb)    (uint8_t)((rgb) & kRed8)
#define G32(rgb)    (uint8_t)((((rgb) & kGreen8) >> 8) & 0xff)
#define B32(rgb)    (uint8_t)((((rgb) & kBlue8) >> 16) & 0xff)
#define RGB565(r, g, b) (uint16_t)(((((uint16_t)(b) << 6) | (g)) << 5) | (r))
#define RGB32(r, g, b) (uint32_t)(((((uint32_t)(b) << 8) | (g)) << 8) | (r))
#else   
#define R16(rgb)    (uint8_t)(((rgb) & kRed5) >> 11)
#define G16(rgb)    (uint8_t)(((rgb) & kGreen6) >> 5)
#define B16(rgb)    (uint8_t)((rgb) & kBlue5)
#define R16_32(rgb) (uint8_t)((((rgb) & kRed5) >> 8) | (((rgb) & kRed5) >> 14))
#define G16_32(rgb) (uint8_t)((((rgb) & kGreen6) >> 3) | (((rgb) & kGreen6) >> 9))
#define B16_32(rgb) (uint8_t)((((rgb) & kBlue5) << 3) | (((rgb) & kBlue5) >> 2))
#define R32(rgb)    (uint8_t)(((rgb) & kRed8) >> 16)
#define G32(rgb)    (uint8_t)(((rgb) & kGreen8) >> 8)
#define B32(rgb)    (uint8_t)((rgb) & kBlue8)
#define RGB565(r, g, b) (uint16_t)(((((uint16_t)(r) << 6) | (g)) << 5) | (b))
#define RGB32(r, g, b) (uint32_t)(((((uint32_t)(r) << 8) | (g)) << 8) | (b))
#endif  


#define kBayer8     0xff
#define kBayer10    0x3ff
#define kBayer12    0xfff

typedef union RGB32_t {
    uint32_t    color;
    struct {
#ifndef HOST_WORDS_BIGENDIAN
        uint8_t r; uint8_t g; uint8_t b; uint8_t a;
#else   
        uint8_t a; uint8_t b; uint8_t g; uint8_t r;
#endif  
    };
} RGB32_t;

static __inline__ int
clamp(int x)
{
    if (x > 255) return 255;
    if (x < 0)   return 0;
    return x;
}


#define RGB2Y(r, g, b) (uint8_t)(((66 * (r) + 129 * (g) +  25 * (b) + 128) >> 8) +  16)
#define RGB2U(r, g, b) (uint8_t)(((-38 * (r) - 74 * (g) + 112 * (b) + 128) >> 8) + 128)
#define RGB2V(r, g, b) (uint8_t)(((112 * (r) - 94 * (g) -  18 * (b) + 128) >> 8) + 128)

static __inline__ void
R8G8B8ToYUV(uint8_t r, uint8_t g, uint8_t b, uint8_t* y, uint8_t* u, uint8_t* v)
{
    *y = RGB2Y((int)r, (int)g, (int)b);
    *u = RGB2U((int)r, (int)g, (int)b);
    *v = RGB2V((int)r, (int)g, (int)b);
}

static __inline__ void
RGB565ToYUV(uint16_t rgb, uint8_t* y, uint8_t* u, uint8_t* v)
{
    R8G8B8ToYUV(R16_32(rgb), G16_32(rgb), B16_32(rgb), y, u, v);
}

static __inline__ void
RGB32ToYUV(uint32_t rgb, uint8_t* y, uint8_t* u, uint8_t* v)
{
    RGB32_t rgb_c;
    rgb_c.color = rgb;
    R8G8B8ToYUV(rgb_c.r, rgb_c.g, rgb_c.b, y, u, v);
}



#define YUV2RO(C, D, E) clamp((298 * (C) + 409 * (E) + 128) >> 8)
#define YUV2GO(C, D, E) clamp((298 * (C) - 100 * (D) - 208 * (E) + 128) >> 8)
#define YUV2BO(C, D, E) clamp((298 * (C) + 516 * (D) + 128) >> 8)

#define YUV2R(y, u, v) clamp((298 * ((y)-16) + 409 * ((v)-128) + 128) >> 8)
#define YUV2G(y, u, v) clamp((298 * ((y)-16) - 100 * ((u)-128) - 208 * ((v)-128) + 128) >> 8)
#define YUV2B(y, u, v) clamp((298 * ((y)-16) + 516 * ((u)-128) + 128) >> 8)


static __inline__ uint16_t
YUVToRGB565(int y, int u, int v)
{
    
    y -= 16; u -= 128; v -= 128;
    const uint16_t r = YUV2RO(y,u,v) >> 3;
    const uint16_t g = YUV2GO(y,u,v) >> 2;
    const uint16_t b = YUV2BO(y,u,v) >> 3;
    return RGB565(r, g, b);
}

static __inline__ uint32_t
YUVToRGB32(int y, int u, int v)
{
    
    y -= 16; u -= 128; v -= 128;
    RGB32_t rgb;
    rgb.r = YUV2RO(y,u,v);
    rgb.g = YUV2GO(y,u,v);
    rgb.b = YUV2BO(y,u,v);
    return rgb.color;
}

static __inline__ void
YUVToRGBPix(int y, int u, int v, uint8_t* r, uint8_t* g, uint8_t* b)
{
    
    y -= 16; u -= 128; v -= 128;
    *r = (uint8_t)YUV2RO(y,u,v);
    *g = (uint8_t)YUV2GO(y,u,v);
    *b = (uint8_t)YUV2BO(y,u,v);
}

static __inline__ uint8_t
_change_exposure(uint8_t inputY, float exp_comp)
{
    return (uint8_t)clamp((float)inputY * exp_comp);
}

static __inline__ void
_change_exposure_RGB(uint8_t* r, uint8_t* g, uint8_t* b, float exp_comp)
{
    uint8_t y, u, v;
    R8G8B8ToYUV(*r, *g, *b, &y, &u, &v);
    YUVToRGBPix(_change_exposure(y, exp_comp), u, v, r, g, b);
}

static __inline__ void
_change_exposure_RGB_i(int* r, int* g, int* b, float exp_comp)
{
    uint8_t y, u, v;
    R8G8B8ToYUV(*r, *g, *b, &y, &u, &v);
    y = _change_exposure(y, exp_comp);
    *r = YUV2RO(y,u,v);
    *g = YUV2GO(y,u,v);
    *b = YUV2BO(y,u,v);
}

static __inline__ void
_change_white_balance_YUV(uint8_t* y,
                          uint8_t* u,
                          uint8_t* v,
                          float r_scale,
                          float g_scale,
                          float b_scale)
{
    int r = (float)(YUV2R((int)*y, (int)*u, (int)*v)) / r_scale;
    int g = (float)(YUV2G((int)*y, (int)*u, (int)*v)) / g_scale;
    int b = (float)(YUV2B((int)*y, (int)*u, (int)*v)) / b_scale;

    *y = RGB2Y(r, g, b);
    *u = RGB2U(r, g, b);
    *v = RGB2V(r, g, b);
}

static __inline__ void
_change_white_balance_RGB(int* r,
                          int* g,
                          int* b,
                          float r_scale,
                          float g_scale,
                          float b_scale)
{
    *r = (float)*r / r_scale;
    *g = (float)*g / g_scale;
    *b = (float)*b / b_scale;
}

static __inline__ void
_change_white_balance_RGB_b(uint8_t* r,
                            uint8_t* g,
                            uint8_t* b,
                            float r_scale,
                            float g_scale,
                            float b_scale)
{
    *r = (float)*r / r_scale;
    *g = (float)*g / g_scale;
    *b = (float)*b / b_scale;
}



typedef struct RGBDesc RGBDesc;
typedef struct YUVDesc YUVDesc;
typedef struct BayerDesc BayerDesc;

typedef const void* (*load_rgb_func)(const void* rgb,
                                     uint8_t* r,
                                     uint8_t* g,
                                     uint8_t* b);

typedef void* (*save_rgb_func)(void* rgb, uint8_t r, uint8_t g, uint8_t b);

typedef int (*u_offset_func)(const YUVDesc* desc, int line, int width, int height);

typedef int (*v_offset_func)(const YUVDesc* desc, int line, int width, int height);

struct RGBDesc {
    
    load_rgb_func   load_rgb;
    
    save_rgb_func   save_rgb;
    
    int             rgb_inc;
};

struct YUVDesc {
    
    int             Y_offset;
    int             Y_inc;
    int             Y_next_pair;
    
    int             UV_inc;
    int             U_offset;
    int             V_offset;
    u_offset_func   u_offset;
    v_offset_func   v_offset;
};

struct BayerDesc {
    const char* color_order;
    int         mask;
};


static const void*
_load_RGB32(const void* rgb, uint8_t* r, uint8_t* g, uint8_t* b)
{
    const uint8_t* rgb_ptr = (const uint8_t*)rgb;
    *r = rgb_ptr[0]; *g = rgb_ptr[1]; *b = rgb_ptr[2];
    return rgb_ptr + 4;
}

static void*
_save_RGB32(void* rgb, uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t* rgb_ptr = (uint8_t*)rgb;
    rgb_ptr[0] = r; rgb_ptr[1] = g; rgb_ptr[2] = b;
    return rgb_ptr + 4;
}

static const void*
_load_BRG32(const void* rgb, uint8_t* r, uint8_t* g, uint8_t* b)
{
    const uint8_t* rgb_ptr = (const uint8_t*)rgb;
    *r = rgb_ptr[2]; *g = rgb_ptr[1]; *b = rgb_ptr[0];
    return rgb_ptr + 4;
}

static void*
_save_BRG32(void* rgb, uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t* rgb_ptr = (uint8_t*)rgb;
    rgb_ptr[2] = r; rgb_ptr[1] = g; rgb_ptr[0] = b;
    return rgb_ptr + 4;
}

static const void*
_load_RGB24(const void* rgb, uint8_t* r, uint8_t* g, uint8_t* b)
{
    const uint8_t* rgb_ptr = (const uint8_t*)rgb;
    *r = rgb_ptr[0]; *g = rgb_ptr[1]; *b = rgb_ptr[2];
    return rgb_ptr + 3;
}

static void*
_save_RGB24(void* rgb, uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t* rgb_ptr = (uint8_t*)rgb;
    rgb_ptr[0] = r; rgb_ptr[1] = g; rgb_ptr[2] = b;
    return rgb_ptr + 3;
}

static const void*
_load_BRG24(const void* rgb, uint8_t* r, uint8_t* g, uint8_t* b)
{
    const uint8_t* rgb_ptr = (const uint8_t*)rgb;
    *r = rgb_ptr[2]; *g = rgb_ptr[1]; *b = rgb_ptr[0];
    return rgb_ptr + 3;
}

static void*
_save_BRG24(void* rgb, uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t* rgb_ptr = (uint8_t*)rgb;
    rgb_ptr[2] = r; rgb_ptr[1] = g; rgb_ptr[0] = b;
    return rgb_ptr + 3;
}

static const void*
_load_RGB16(const void* rgb, uint8_t* r, uint8_t* g, uint8_t* b)
{
    const uint16_t rgb16 = *(const uint16_t*)rgb;
    *r = R16(rgb16); *g = G16(rgb16); *b = B16(rgb16);
    return (const uint8_t*)rgb + 2;
}

static void*
_save_RGB16(void* rgb, uint8_t r, uint8_t g, uint8_t b)
{
    *(uint16_t*)rgb = RGB565(r & 0x1f, g & 0x3f, b & 0x1f);
    return (uint8_t*)rgb + 2;
}

static const void*
_load_BRG16(const void* rgb, uint8_t* r, uint8_t* g, uint8_t* b)
{
    const uint16_t rgb16 = *(const uint16_t*)rgb;
    *r = B16(rgb16); *g = G16(rgb16); *b = R16(rgb16);
    return (const uint8_t*)rgb + 2;
}

static void*
_save_BRG16(void* rgb, uint8_t r, uint8_t g, uint8_t b)
{
    *(uint16_t*)rgb = RGB565(b & 0x1f, g & 0x3f, r & 0x1f);
    return (uint8_t*)rgb + 2;
}


static int
_UOffIntrlYUV(const YUVDesc* desc, int line, int width, int height)
{
    return line * width * 2 + desc->U_offset;
}

static int
_VOffIntrlYUV(const YUVDesc* desc, int line, int width, int height)
{
    
    return line * width * 2 + desc->V_offset;
}

static int
_UOffIntrlUV(const YUVDesc* desc, int line, int width, int height)
{
    return (height + line / 2) * width + desc->U_offset;
}

static int
_VOffIntrlUV(const YUVDesc* desc, int line, int width, int height)
{
    
    return (height + line / 2) * width + desc->V_offset;
}

static int
_UOffSepYUV(const YUVDesc* desc, int line, int width, int height)
{
    const int y_pane_size = height * width;
    if (desc->U_offset) {
        
        return y_pane_size + (line / 2) * width / 2;
    } else {
        
        return y_pane_size + y_pane_size / 4 + (line / 2) * width / 2;
    }
}

static int
_VOffSepYUV(const YUVDesc* desc, int line, int width, int height)
{
    
    const int y_pane_size = height * width;
    if (desc->V_offset) {
        
        return y_pane_size + (line / 2) * width / 2;
    } else {
        
        return y_pane_size + y_pane_size / 4 + (line / 2) * width / 2;
    }
}


static __inline__ int
_get_bayer_color(const BayerDesc* desc, const void* buf, int x, int y, int width)
{
    if (desc->mask == kBayer8) {
        
        return *((const uint8_t*)buf + y * width + x);
    } else {
#ifndef HOST_WORDS_BIGENDIAN
        return *((const int16_t*)buf + y * width + x) & desc->mask;
#else
        const uint8_t* pixel = (const uint8_t*)buf + (y * width + x) * 2;
        return (((uint16_t)pixel[1] << 8) | pixel[0]) & desc->mask;
#endif  
    }
}

static int
_get_bayer_ave_hor(const BayerDesc* desc,
                   const void* buf,
                   int x,
                   int y,
                   int width,
                   int height)
{
    if (x == 0) {
        return _get_bayer_color(desc, buf, x + 1, y, width);
    } else if (x == (width - 1)) {
        return _get_bayer_color(desc, buf, x - 1, y, width);
    } else {
        return (_get_bayer_color(desc, buf, x - 1, y, width) +
                _get_bayer_color(desc, buf, x + 1, y, width)) / 2;
    }
}

static int
_get_bayer_ave_vert(const BayerDesc* desc,
                    const void* buf,
                    int x,
                    int y,
                    int width,
                    int height)
{
    if (y == 0) {
        return _get_bayer_color(desc, buf, x, y + 1, width);
    } else if (y == (height - 1)) {
        return _get_bayer_color(desc, buf, x, y - 1, width);
    } else {
        return (_get_bayer_color(desc, buf, x, y - 1, width) +
                _get_bayer_color(desc, buf, x, y + 1, width)) / 2;
    }
}

static int
_get_bayer_ave_cross(const BayerDesc* desc,
                     const void* buf,
                     int x,
                     int y,
                     int width,
                     int height)
{
    if (x > 0 && x < (width - 1) && y > 0 && y < (height - 1)) {
        return (_get_bayer_color(desc, buf, x - 1, y, width) +
                _get_bayer_color(desc, buf, x + 1, y, width) +
                _get_bayer_color(desc, buf, x, y - 1, width) +
                _get_bayer_color(desc, buf, x, y + 1, width)) / 4;
    } else {
        int sum = 0;
        int num = 0;

        
        if (x == 0) {
            sum += _get_bayer_color(desc, buf, x + 1, y, width);
            num++;
        } else if (x == (width - 1)) {
            sum += _get_bayer_color(desc, buf, x - 1, y, width);
            num++;
        } else {
            sum += _get_bayer_color(desc, buf, x - 1, y, width) +
                   _get_bayer_color(desc, buf, x + 1, y, width);
            num += 2;
        }

        
        if (y == 0) {
            sum += _get_bayer_color(desc, buf, x, y + 1, width);
            num++;
        } else if (y == (height - 1)) {
            sum += _get_bayer_color(desc, buf, x, y - 1, width);
            num++;
        } else {
            sum += _get_bayer_color(desc, buf, x, y - 1, width) +
                   _get_bayer_color(desc, buf, x, y + 1, width);
            num += 2;
        }

        return sum / num;
    }
}

static int
_get_bayer_ave_diag(const BayerDesc* desc,
                    const void* buf,
                    int x,
                    int y,
                    int width,
                    int height)
{
    if (x > 0 && x < (width - 1) && y > 0 && y < (height - 1)) {
        return (_get_bayer_color(desc, buf, x - 1, y - 1, width) +
                _get_bayer_color(desc, buf, x + 1, y - 1, width) +
                _get_bayer_color(desc, buf, x - 1, y + 1, width) +
                _get_bayer_color(desc, buf, x + 1, y + 1, width)) / 4;
    } else {
        int sum = 0;
        int num = 0;
        int xx, yy;
        for (xx = x - 1; xx < (x + 2); xx += 2) {
            for (yy = y - 1; yy < (y + 2); yy += 2) {
                if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
                    sum += _get_bayer_color(desc, buf, xx, yy, width);
                    num++;
                }
            }
        }
        return sum / num;
    }
}

static __inline__ char
_get_bayer_color_sel(const BayerDesc* desc, int x, int y)
{
    return desc->color_order[((y & 1) << 1) | (x & 1)];
}

static void
_get_bayerRGB(const BayerDesc* desc,
              const void* buf,
              int x,
              int y,
              int width,
              int height,
              int* red,
              int* green,
              int* blue)
{
    const char pixel_color = _get_bayer_color_sel(desc, x, y);

    if (pixel_color == 'G') {
        
        const char next_pixel_color = _get_bayer_color_sel(desc, x + 1, y);
        *green = _get_bayer_color(desc, buf, x, y, width);
        if (next_pixel_color == 'R') {
            *red = _get_bayer_ave_hor(desc, buf, x, y, width, height);
            *blue = _get_bayer_ave_vert(desc, buf, x, y, width, height);
        } else {
            *red = _get_bayer_ave_vert(desc, buf, x, y, width, height);
            *blue = _get_bayer_ave_hor(desc, buf, x, y, width, height);
        }
    } else if (pixel_color == 'R') {
        
        *red = _get_bayer_color(desc, buf, x, y, width);
        *green = _get_bayer_ave_cross(desc, buf, x, y, width, height);
        *blue = _get_bayer_ave_diag(desc, buf, x, y, width, height);
    } else {
        
        *blue = _get_bayer_color(desc, buf, x, y, width);
        *green = _get_bayer_ave_cross(desc, buf, x, y, width, height);
        *red = _get_bayer_ave_diag(desc, buf, x, y, width, height);
    }
}


static void
RGBToYUV(const RGBDesc* rgb_fmt,
         const YUVDesc* yuv_fmt,
         const void* rgb,
         void* yuv,
         int width,
         int height,
         float r_scale,
         float g_scale,
         float b_scale,
         float exp_comp)
{
    int y, x;
    const int Y_Inc = yuv_fmt->Y_inc;
    const int UV_inc = yuv_fmt->UV_inc;
    const int Y_next_pair = yuv_fmt->Y_next_pair;
    uint8_t* pY = (uint8_t*)yuv + yuv_fmt->Y_offset;
    for (y = 0; y < height; y++) {
        uint8_t* pU =
            (uint8_t*)yuv + yuv_fmt->u_offset(yuv_fmt, y, width, height);
        uint8_t* pV =
            (uint8_t*)yuv + yuv_fmt->v_offset(yuv_fmt, y, width, height);
        for (x = 0; x < width; x += 2,
                               pY += Y_next_pair, pU += UV_inc, pV += UV_inc) {
            uint8_t r, g, b;
            rgb = rgb_fmt->load_rgb(rgb, &r, &g, &b);
            _change_white_balance_RGB_b(&r, &g, &b, r_scale, g_scale, b_scale);
            _change_exposure_RGB(&r, &g, &b, exp_comp);
            R8G8B8ToYUV(r, g, b, pY, pU, pV);
            rgb = rgb_fmt->load_rgb(rgb, &r, &g, &b);
            _change_white_balance_RGB_b(&r, &g, &b, r_scale, g_scale, b_scale);
            _change_exposure_RGB(&r, &g, &b, exp_comp);
            pY[Y_Inc] = RGB2Y((int)r, (int)g, (int)b);
        }
        
        if (((uintptr_t)rgb & 1) != 0) rgb = (const uint8_t*)rgb + 1;
    }
}

static void
RGBToRGB(const RGBDesc* src_rgb_fmt,
         const RGBDesc* dst_rgb_fmt,
         const void* src_rgb,
         void* dst_rgb,
         int width,
         int height,
         float r_scale,
         float g_scale,
         float b_scale,
         float exp_comp)
{
    int x, y;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            uint8_t r, g, b;
            src_rgb = src_rgb_fmt->load_rgb(src_rgb, &r, &g, &b);
            _change_white_balance_RGB_b(&r, &g, &b, r_scale, g_scale, b_scale);
            _change_exposure_RGB(&r, &g, &b, exp_comp);
            dst_rgb = dst_rgb_fmt->save_rgb(dst_rgb, r, g, b);
        }
        
        if (((uintptr_t)src_rgb & 1) != 0) src_rgb = (uint8_t*)src_rgb + 1;
        if (((uintptr_t)dst_rgb & 1) != 0) dst_rgb = (uint8_t*)dst_rgb + 1;
    }
}

static void
YUVToRGB(const YUVDesc* yuv_fmt,
         const RGBDesc* rgb_fmt,
         const void* yuv,
         void* rgb,
         int width,
         int height,
         float r_scale,
         float g_scale,
         float b_scale,
         float exp_comp)
{
    int y, x;
    const int Y_Inc = yuv_fmt->Y_inc;
    const int UV_inc = yuv_fmt->UV_inc;
    const int Y_next_pair = yuv_fmt->Y_next_pair;
    const uint8_t* pY = (const uint8_t*)yuv + yuv_fmt->Y_offset;
    for (y = 0; y < height; y++) {
        const uint8_t* pU =
            (const uint8_t*)yuv + yuv_fmt->u_offset(yuv_fmt, y, width, height);
        const uint8_t* pV =
            (const uint8_t*)yuv + yuv_fmt->v_offset(yuv_fmt, y, width, height);
        for (x = 0; x < width; x += 2,
                               pY += Y_next_pair, pU += UV_inc, pV += UV_inc) {
            uint8_t r, g, b;
            const uint8_t U = *pU;
            const uint8_t V = *pV;
            YUVToRGBPix(*pY, U, V, &r, &g, &b);
            _change_white_balance_RGB_b(&r, &g, &b, r_scale, g_scale, b_scale);
            _change_exposure_RGB(&r, &g, &b, exp_comp);
            rgb = rgb_fmt->save_rgb(rgb, r, g, b);
            YUVToRGBPix(pY[Y_Inc], U, V, &r, &g, &b);
            _change_white_balance_RGB_b(&r, &g, &b, r_scale, g_scale, b_scale);
            _change_exposure_RGB(&r, &g, &b, exp_comp);
            rgb = rgb_fmt->save_rgb(rgb, r, g, b);
        }
        
        if (((uintptr_t)rgb & 1) != 0) rgb = (uint8_t*)rgb + 1;
    }
}

static void
YUVToYUV(const YUVDesc* src_fmt,
         const YUVDesc* dst_fmt,
         const void* src,
         void* dst,
         int width,
         int height,
         float r_scale,
         float g_scale,
         float b_scale,
         float exp_comp)
{
    int y, x;
    const int Y_Inc_src = src_fmt->Y_inc;
    const int UV_inc_src = src_fmt->UV_inc;
    const int Y_next_pair_src = src_fmt->Y_next_pair;
    const int Y_Inc_dst = dst_fmt->Y_inc;
    const int UV_inc_dst = dst_fmt->UV_inc;
    const int Y_next_pair_dst = dst_fmt->Y_next_pair;
    const uint8_t* pYsrc = (const uint8_t*)src + src_fmt->Y_offset;
    uint8_t* pYdst = (uint8_t*)dst + dst_fmt->Y_offset;
    for (y = 0; y < height; y++) {
        const uint8_t* pUsrc =
            (const uint8_t*)src + src_fmt->u_offset(src_fmt, y, width, height);
        const uint8_t* pVsrc =
            (const uint8_t*)src + src_fmt->v_offset(src_fmt, y, width, height);
        uint8_t* pUdst =
            (uint8_t*)dst + dst_fmt->u_offset(dst_fmt, y, width, height);
        uint8_t* pVdst =
            (uint8_t*)dst + dst_fmt->v_offset(dst_fmt, y, width, height);
        for (x = 0; x < width; x += 2, pYsrc += Y_next_pair_src,
                                       pUsrc += UV_inc_src,
                                       pVsrc += UV_inc_src,
                                       pYdst += Y_next_pair_dst,
                                       pUdst += UV_inc_dst,
                                       pVdst += UV_inc_dst) {
            *pYdst = *pYsrc; *pUdst = *pUsrc; *pVdst = *pVsrc;
            _change_white_balance_YUV(pYdst, pUdst, pVdst, r_scale, g_scale, b_scale);
            *pYdst = _change_exposure(*pYdst, exp_comp);
            pYdst[Y_Inc_dst] = _change_exposure(pYsrc[Y_Inc_src], exp_comp);
        }
    }
}

static void
BAYERToRGB(const BayerDesc* bayer_fmt,
           const RGBDesc* rgb_fmt,
           const void* bayer,
           void* rgb,
           int width,
           int height,
           float r_scale,
           float g_scale,
           float b_scale,
           float exp_comp)
{
    int y, x;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            int r, g, b;
            _get_bayerRGB(bayer_fmt, bayer, x, y, width, height, &r, &g, &b);
            if (bayer_fmt->mask == kBayer10) {
                r >>= 2; g >>= 2; b >>= 2;
            } else if (bayer_fmt->mask == kBayer12) {
                r >>= 4; g >>= 4; b >>= 4;
            }
            _change_white_balance_RGB(&r, &g, &b, r_scale, g_scale, b_scale);
            _change_exposure_RGB_i(&r, &g, &b, exp_comp);
            rgb = rgb_fmt->save_rgb(rgb, r, g, b);
        }
        
        if (((uintptr_t)rgb & 1) != 0) rgb = (uint8_t*)rgb + 1;
    }
}

static void
BAYERToYUV(const BayerDesc* bayer_fmt,
           const YUVDesc* yuv_fmt,
           const void* bayer,
           void* yuv,
           int width,
           int height,
           float r_scale,
           float g_scale,
           float b_scale,
           float exp_comp)
{
    int y, x;
    const int Y_Inc = yuv_fmt->Y_inc;
    const int UV_inc = yuv_fmt->UV_inc;
    const int Y_next_pair = yuv_fmt->Y_next_pair;
    uint8_t* pY = (uint8_t*)yuv + yuv_fmt->Y_offset;
    for (y = 0; y < height; y++) {
        uint8_t* pU =
            (uint8_t*)yuv + yuv_fmt->u_offset(yuv_fmt, y, width, height);
        uint8_t* pV =
            (uint8_t*)yuv + yuv_fmt->v_offset(yuv_fmt, y, width, height);
        for (x = 0; x < width; x += 2,
                               pY += Y_next_pair, pU += UV_inc, pV += UV_inc) {
            int r, g, b;
            _get_bayerRGB(bayer_fmt, bayer, x, y, width, height, &r, &g, &b);
            _change_white_balance_RGB(&r, &g, &b, r_scale, g_scale, b_scale);
            _change_exposure_RGB_i(&r, &g, &b, exp_comp);
            R8G8B8ToYUV(r, g, b, pY, pU, pV);
            _get_bayerRGB(bayer_fmt, bayer, x + 1, y, width, height, &r, &g, &b);
            _change_white_balance_RGB(&r, &g, &b, r_scale, g_scale, b_scale);
            _change_exposure_RGB_i(&r, &g, &b, exp_comp);
            pY[Y_Inc] = RGB2Y(r, g, b);
        }
    }
}


static const RGBDesc _RGB32 =
{
    .load_rgb   = _load_RGB32,
    .save_rgb   = _save_RGB32,
    .rgb_inc    = 4
};

static const RGBDesc _BRG32 =
{
    .load_rgb   = _load_BRG32,
    .save_rgb   = _save_BRG32,
    .rgb_inc    = 4
};

static const RGBDesc _RGB24 =
{
    .load_rgb   = _load_RGB24,
    .save_rgb   = _save_RGB24,
    .rgb_inc    = 3
};

static const RGBDesc _BRG24 =
{
    .load_rgb   = _load_BRG24,
    .save_rgb   = _save_BRG24,
    .rgb_inc    = 3
};

static const RGBDesc _RGB16 =
{
    .load_rgb   = _load_RGB16,
    .save_rgb   = _save_RGB16,
    .rgb_inc    = 2
};

static const RGBDesc _BRG16 =
{
    .load_rgb   = _load_BRG16,
    .save_rgb   = _save_BRG16,
    .rgb_inc    = 2
};


static const YUVDesc _YUYV =
{
    .Y_offset       = 0,
    .Y_inc          = 2,
    .Y_next_pair    = 4,
    .UV_inc         = 4,
    .U_offset       = 1,
    .V_offset       = 3,
    .u_offset       = &_UOffIntrlYUV,
    .v_offset       = &_VOffIntrlYUV
};

static const YUVDesc _UYVY =
{
    .Y_offset       = 1,
    .Y_inc          = 2,
    .Y_next_pair    = 4,
    .UV_inc         = 4,
    .U_offset       = 0,
    .V_offset       = 2,
    .u_offset       = &_UOffIntrlYUV,
    .v_offset       = &_VOffIntrlYUV
};

static const YUVDesc _YVYU =
{
    .Y_offset       = 0,
    .Y_inc          = 2,
    .Y_next_pair    = 4,
    .UV_inc         = 4,
    .U_offset       = 3,
    .V_offset       = 1,
    .u_offset       = &_UOffIntrlYUV,
    .v_offset       = &_VOffIntrlYUV
};

static const YUVDesc _VYUY =
{
    .Y_offset       = 1,
    .Y_inc          = 2,
    .Y_next_pair    = 4,
    .UV_inc         = 4,
    .U_offset       = 2,
    .V_offset       = 0,
    .u_offset       = &_UOffIntrlYUV,
    .v_offset       = &_VOffIntrlYUV
};

static const YUVDesc _YYUV =
{
    .Y_offset       = 0,
    .Y_inc          = 1,
    .Y_next_pair    = 4,
    .UV_inc         = 4,
    .U_offset       = 2,
    .V_offset       = 3,
    .u_offset       = &_UOffIntrlYUV,
    .v_offset       = &_VOffIntrlYUV
};

static const YUVDesc _YYVU =
{
    .Y_offset       = 0,
    .Y_inc          = 1,
    .Y_next_pair    = 4,
    .UV_inc         = 4,
    .U_offset       = 3,
    .V_offset       = 2,
    .u_offset       = &_UOffIntrlYUV,
    .v_offset       = &_VOffIntrlYUV
};


static const YUVDesc _YV12 =
{
    .Y_offset       = 0,
    .Y_inc          = 1,
    .Y_next_pair    = 2,
    .UV_inc         = 1,
    .U_offset       = 0,
    .V_offset       = 1,
    .u_offset       = &_UOffSepYUV,
    .v_offset       = &_VOffSepYUV
};

static const YUVDesc _YU12 =
{
    .Y_offset       = 0,
    .Y_inc          = 1,
    .Y_next_pair    = 2,
    .UV_inc         = 1,
    .U_offset       = 1,
    .V_offset       = 0,
    .u_offset       = &_UOffSepYUV,
    .v_offset       = &_VOffSepYUV
};

static const YUVDesc _NV12 =
{
    .Y_offset       = 0,
    .Y_inc          = 1,
    .Y_next_pair    = 2,
    .UV_inc         = 2,
    .U_offset       = 0,
    .V_offset       = 1,
    .u_offset       = &_UOffIntrlUV,
    .v_offset       = &_VOffIntrlUV
};

static const YUVDesc _NV21 =
{
    .Y_offset       = 0,
    .Y_inc          = 1,
    .Y_next_pair    = 2,
    .UV_inc         = 2,
    .U_offset       = 1,
    .V_offset       = 0,
    .u_offset       = &_UOffIntrlUV,
    .v_offset       = &_VOffIntrlUV
};


static const BayerDesc _GB8 =
{
    .mask           = kBayer8,
    .color_order    = "GBRG"
};

static const BayerDesc _GR8 =
{
    .mask           = kBayer8,
    .color_order    = "GRBG"
};

static const BayerDesc _BG8 =
{
    .mask           = kBayer8,
    .color_order    = "BGGR"
};

static const BayerDesc _RG8 =
{
    .mask           = kBayer8,
    .color_order    = "RGGB"
};

static const BayerDesc _GB10 =
{
    .mask           = kBayer10,
    .color_order    = "GBRG"
};

static const BayerDesc _GR10 =
{
    .mask           = kBayer10,
    .color_order    = "GRBG"
};

static const BayerDesc _BG10 =
{
    .mask           = kBayer10,
    .color_order    = "BGGR"
};

static const BayerDesc _RG10 =
{
    .mask           = kBayer10,
    .color_order    = "RGGB"
};

static const BayerDesc _GB12 =
{
    .mask           = kBayer12,
    .color_order    = "GBRG"
};

static const BayerDesc _GR12 =
{
    .mask           = kBayer12,
    .color_order    = "GRBG"
};

static const BayerDesc _BG12 =
{
    .mask           = kBayer12,
    .color_order    = "BGGR"
};

static const BayerDesc _RG12 =
{
    .mask           = kBayer12,
    .color_order    = "RGGB"
};



typedef enum PIXFormatSel {
    
    PIX_FMT_RGB,
    
    PIX_FMT_YUV,
    
    PIX_FMT_BAYER
} PIXFormatSel;

typedef struct PIXFormat {
    
    uint32_t        fourcc_type;
    
    PIXFormatSel    format_sel;
    union {
        
        const RGBDesc*      rgb_desc;
        
        const YUVDesc*      yuv_desc;
        
        const BayerDesc*    bayer_desc;
    } desc;
} PIXFormat;

static const PIXFormat _PIXFormats[] = {
    
    { V4L2_PIX_FMT_RGB32,   PIX_FMT_RGB,    .desc.rgb_desc = &_RGB32  },
    { V4L2_PIX_FMT_BGR32,   PIX_FMT_RGB,    .desc.rgb_desc = &_BRG32  },
    { V4L2_PIX_FMT_RGB565,  PIX_FMT_RGB,    .desc.rgb_desc = &_RGB16  },
    { V4L2_PIX_FMT_RGB24,   PIX_FMT_RGB,    .desc.rgb_desc = &_RGB24  },
    { V4L2_PIX_FMT_BGR24,   PIX_FMT_RGB,    .desc.rgb_desc = &_BRG24  },

    
    { V4L2_PIX_FMT_YVU420,  PIX_FMT_YUV,    .desc.yuv_desc = &_YV12   },
    { V4L2_PIX_FMT_YUV420,  PIX_FMT_YUV,    .desc.yuv_desc = &_YU12   },
    { V4L2_PIX_FMT_NV12,    PIX_FMT_YUV,    .desc.yuv_desc = &_NV12   },
    { V4L2_PIX_FMT_NV21,    PIX_FMT_YUV,    .desc.yuv_desc = &_NV21   },

    
    { V4L2_PIX_FMT_YUYV,    PIX_FMT_YUV,    .desc.yuv_desc = &_YUYV   },
    { V4L2_PIX_FMT_YYUV,    PIX_FMT_YUV,    .desc.yuv_desc = &_YYUV   },
    { V4L2_PIX_FMT_YVYU,    PIX_FMT_YUV,    .desc.yuv_desc = &_YVYU   },
    { V4L2_PIX_FMT_UYVY,    PIX_FMT_YUV,    .desc.yuv_desc = &_UYVY   },
    { V4L2_PIX_FMT_VYUY,    PIX_FMT_YUV,    .desc.yuv_desc = &_VYUY   },
    { V4L2_PIX_FMT_YVYU,    PIX_FMT_YUV,    .desc.yuv_desc = &_YVYU   },
    { V4L2_PIX_FMT_VYUY,    PIX_FMT_YUV,    .desc.yuv_desc = &_VYUY   },
    { V4L2_PIX_FMT_YYVU,    PIX_FMT_YUV,    .desc.yuv_desc = &_YYVU   },
    { V4L2_PIX_FMT_YUY2,    PIX_FMT_YUV,    .desc.yuv_desc = &_YUYV   },
    { V4L2_PIX_FMT_YUNV,    PIX_FMT_YUV,    .desc.yuv_desc = &_YUYV   },
    { V4L2_PIX_FMT_V422,    PIX_FMT_YUV,    .desc.yuv_desc = &_YUYV   },

    
    { V4L2_PIX_FMT_SBGGR8,  PIX_FMT_BAYER,  .desc.bayer_desc = &_BG8  },
    { V4L2_PIX_FMT_SGBRG8,  PIX_FMT_BAYER,  .desc.bayer_desc = &_GB8  },
    { V4L2_PIX_FMT_SGRBG8,  PIX_FMT_BAYER,  .desc.bayer_desc = &_GR8  },
    { V4L2_PIX_FMT_SRGGB8,  PIX_FMT_BAYER,  .desc.bayer_desc = &_RG8  },
    { V4L2_PIX_FMT_SBGGR10, PIX_FMT_BAYER,  .desc.bayer_desc = &_BG10 },
    { V4L2_PIX_FMT_SGBRG10, PIX_FMT_BAYER,  .desc.bayer_desc = &_GB10 },
    { V4L2_PIX_FMT_SGRBG10, PIX_FMT_BAYER,  .desc.bayer_desc = &_GR10 },
    { V4L2_PIX_FMT_SRGGB10, PIX_FMT_BAYER,  .desc.bayer_desc = &_RG10 },
    { V4L2_PIX_FMT_SBGGR12, PIX_FMT_BAYER,  .desc.bayer_desc = &_BG12 },
    { V4L2_PIX_FMT_SGBRG12, PIX_FMT_BAYER,  .desc.bayer_desc = &_GB12 },
    { V4L2_PIX_FMT_SGRBG12, PIX_FMT_BAYER,  .desc.bayer_desc = &_GR12 },
    { V4L2_PIX_FMT_SRGGB12, PIX_FMT_BAYER,  .desc.bayer_desc = &_RG12 },
};
static const int _PIXFormats_num = sizeof(_PIXFormats) / sizeof(*_PIXFormats);

static const PIXFormat*
_get_pixel_format_descriptor(uint32_t pixel_format)
{
    int f;
    for (f = 0; f < _PIXFormats_num; f++) {
        if (_PIXFormats[f].fourcc_type == pixel_format) {
            return &_PIXFormats[f];
        }
    }
    W("%s: Pixel format %.4s is unknown",
      __FUNCTION__, (const char*)&pixel_format);
    return NULL;
}


int
has_converter(uint32_t from, uint32_t to)
{
    if (from == to) {
        
        return 1;
    }
    return _get_pixel_format_descriptor(from) != NULL &&
           _get_pixel_format_descriptor(to) != NULL;
}

int
convert_frame(const void* frame,
              uint32_t pixel_format,
              size_t framebuffer_size,
              int width,
              int height,
              ClientFrameBuffer* framebuffers,
              int fbs_num,
              float r_scale,
              float g_scale,
              float b_scale,
              float exp_comp)
{
    int n;
    const PIXFormat* src_desc = _get_pixel_format_descriptor(pixel_format);
    if (src_desc == NULL) {
        E("%s: Source pixel format %.4s is unknown",
          __FUNCTION__, (const char*)&pixel_format);
        return -1;
    }

    for (n = 0; n < fbs_num; n++) {
        const PIXFormat* dst_desc =
            _get_pixel_format_descriptor(framebuffers[n].pixel_format);
        if (dst_desc == NULL) {
            E("%s: Destination pixel format %.4s is unknown",
              __FUNCTION__, (const char*)&framebuffers[n].pixel_format);
            return -1;
        }
        switch (src_desc->format_sel) {
            case PIX_FMT_RGB:
                if (dst_desc->format_sel == PIX_FMT_RGB) {
                    RGBToRGB(src_desc->desc.rgb_desc, dst_desc->desc.rgb_desc,
                             frame, framebuffers[n].framebuffer, width, height,
                             r_scale, g_scale, b_scale, exp_comp);
                } else if (dst_desc->format_sel == PIX_FMT_YUV) {
                    RGBToYUV(src_desc->desc.rgb_desc, dst_desc->desc.yuv_desc,
                             frame, framebuffers[n].framebuffer, width, height,
                             r_scale, g_scale, b_scale, exp_comp);
                } else {
                    E("%s: Unexpected destination pixel format %d",
                      __FUNCTION__, dst_desc->format_sel);
                    return -1;
                }
                break;
            case PIX_FMT_YUV:
                if (dst_desc->format_sel == PIX_FMT_RGB) {
                    YUVToRGB(src_desc->desc.yuv_desc, dst_desc->desc.rgb_desc,
                             frame, framebuffers[n].framebuffer, width, height,
                             r_scale, g_scale, b_scale, exp_comp);
                } else if (dst_desc->format_sel == PIX_FMT_YUV) {
                    YUVToYUV(src_desc->desc.yuv_desc, dst_desc->desc.yuv_desc,
                             frame, framebuffers[n].framebuffer, width, height,
                             r_scale, g_scale, b_scale, exp_comp);
                } else {
                    E("%s: Unexpected destination pixel format %d",
                      __FUNCTION__, dst_desc->format_sel);
                    return -1;
                }
                break;
            case PIX_FMT_BAYER:
                if (dst_desc->format_sel == PIX_FMT_RGB) {
                    BAYERToRGB(src_desc->desc.bayer_desc, dst_desc->desc.rgb_desc,
                              frame, framebuffers[n].framebuffer, width, height,
                              r_scale, g_scale, b_scale, exp_comp);
                } else if (dst_desc->format_sel == PIX_FMT_YUV) {
                    BAYERToYUV(src_desc->desc.bayer_desc, dst_desc->desc.yuv_desc,
                               frame, framebuffers[n].framebuffer, width, height,
                               r_scale, g_scale, b_scale, exp_comp);
                } else {
                    E("%s: Unexpected destination pixel format %d",
                      __FUNCTION__, dst_desc->format_sel);
                    return -1;
                }
                break;
            default:
                E("%s: Unexpected source pixel format %d",
                  __FUNCTION__, dst_desc->format_sel);
                return -1;
        }
    }

    return 0;
}
