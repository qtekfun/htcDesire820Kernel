#ifndef QCMS_H
#define QCMS_H

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef ICC_H

/***************************************************************** 
 Copyright (c) 1994-1996 SunSoft, Inc.

                    Rights Reserved

Permission is hereby granted, free of charge, to any person 
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restrict- 
ion, including without limitation the rights to use, copy, modify, 
merge, publish distribute, sublicense, and/or sell copies of the 
Software, and to permit persons to whom the Software is furnished 
to do so, subject to the following conditions: 
 
The above copyright notice and this permission notice shall be 
included in all copies or substantial portions of the Software. 
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-
INFRINGEMENT.  IN NO EVENT SHALL SUNSOFT, INC. OR ITS PARENT 
COMPANY BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
OTHER DEALINGS IN THE SOFTWARE. 
 
Except as contained in this notice, the name of SunSoft, Inc. 
shall not be used in advertising or otherwise to promote the 
sale, use or other dealings in this Software without written 
authorization from SunSoft Inc. 
******************************************************************/


 
typedef enum {
    icSigXYZData                        = 0x58595A20L,  
    icSigLabData                        = 0x4C616220L,  
    icSigLuvData                        = 0x4C757620L,  
    icSigYCbCrData                      = 0x59436272L,  
    icSigYxyData                        = 0x59787920L,  
    icSigRgbData                        = 0x52474220L,  
    icSigGrayData                       = 0x47524159L,  
    icSigHsvData                        = 0x48535620L,  
    icSigHlsData                        = 0x484C5320L,  
    icSigCmykData                       = 0x434D594BL,  
    icSigCmyData                        = 0x434D5920L,  
    icSig2colorData                     = 0x32434C52L,  
    icSig3colorData                     = 0x33434C52L,  
    icSig4colorData                     = 0x34434C52L,  
    icSig5colorData                     = 0x35434C52L,  
    icSig6colorData                     = 0x36434C52L,  
    icSig7colorData                     = 0x37434C52L,  
    icSig8colorData                     = 0x38434C52L,  
    icSig9colorData                     = 0x39434C52L,  
    icSig10colorData                    = 0x41434C52L,  
    icSig11colorData                    = 0x42434C52L,  
    icSig12colorData                    = 0x43434C52L,  
    icSig13colorData                    = 0x44434C52L,  
    icSig14colorData                    = 0x45434C52L,  
    icSig15colorData                    = 0x46434C52L,  
    icMaxEnumData                       = 0xFFFFFFFFL   
} icColorSpaceSignature;
#endif

#include <stdio.h>

typedef int qcms_bool;

struct _qcms_transform;
typedef struct _qcms_transform qcms_transform;

struct _qcms_profile;
typedef struct _qcms_profile qcms_profile;

typedef enum {
	QCMS_INTENT_DEFAULT = 0,
	QCMS_INTENT_PERCEPTUAL = 0,
	QCMS_INTENT_RELATIVE_COLORIMETRIC = 1,
	QCMS_INTENT_SATURATION = 2,
	QCMS_INTENT_ABSOLUTE_COLORIMETRIC = 3
} qcms_intent;

typedef enum {
	QCMS_DATA_RGB_8,
	QCMS_DATA_RGBA_8,
	QCMS_DATA_GRAY_8,
	QCMS_DATA_GRAYA_8
} qcms_data_type;

typedef enum {
	QCMS_OUTPUT_RGBX,
	QCMS_OUTPUT_BGRX
} qcms_output_type;

typedef struct
{
	double x;
	double y;
	double Y;
} qcms_CIE_xyY;

typedef struct
{
	qcms_CIE_xyY red;
	qcms_CIE_xyY green;
	qcms_CIE_xyY blue;
} qcms_CIE_xyYTRIPLE;

qcms_profile* qcms_profile_create_rgb_with_gamma(
		qcms_CIE_xyY white_point,
		qcms_CIE_xyYTRIPLE primaries,
		float gamma);

qcms_profile* qcms_profile_from_memory(const void *mem, size_t size);

qcms_profile* qcms_profile_from_file(FILE *file);
qcms_profile* qcms_profile_from_path(const char *path);
#ifdef _WIN32
qcms_profile* qcms_profile_from_unicode_path(const wchar_t *path);
#endif
qcms_profile* qcms_profile_sRGB(void);
void qcms_profile_release(qcms_profile *profile);

qcms_bool qcms_profile_is_bogus(qcms_profile *profile);
qcms_intent qcms_profile_get_rendering_intent(qcms_profile *profile);
icColorSpaceSignature qcms_profile_get_color_space(qcms_profile *profile);

void qcms_profile_precache_output_transform(qcms_profile *profile);

qcms_transform* qcms_transform_create(
		qcms_profile *in, qcms_data_type in_type,
		qcms_profile* out, qcms_data_type out_type,
		qcms_intent intent);

void qcms_transform_release(qcms_transform *);

void qcms_transform_data(qcms_transform *transform, void *src, void *dest, size_t length);
void qcms_transform_data_type(qcms_transform *transform, void *src, void *dest, size_t length, qcms_output_type type);

void qcms_enable_iccv4();

#ifdef  __cplusplus
}
#endif

#endif
