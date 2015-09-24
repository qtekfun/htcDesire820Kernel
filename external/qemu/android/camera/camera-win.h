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

#ifndef ANDROID_CAMERA_CAMERA_WIN_H_
#define ANDROID_CAMERA_CAMERA_WIN_H_


#define v4l2_fourcc(a,b,c,d)\
    (((uint32_t)(a)<<0)|((uint32_t)(b)<<8)|((uint32_t)(c)<<16)|((uint32_t)(d)<<24))

#define V4L2_PIX_FMT_RGB332  v4l2_fourcc('R','G','B','1') 
#define V4L2_PIX_FMT_RGB444  v4l2_fourcc('R','4','4','4') 
#define V4L2_PIX_FMT_RGB555  v4l2_fourcc('R','G','B','O') 
#define V4L2_PIX_FMT_RGB565  v4l2_fourcc('R','G','B','P') 
#define V4L2_PIX_FMT_RGB555X v4l2_fourcc('R','G','B','Q') 
#define V4L2_PIX_FMT_RGB565X v4l2_fourcc('R','G','B','R') 
#define V4L2_PIX_FMT_BGR24   v4l2_fourcc('B','G','R','3') 
#define V4L2_PIX_FMT_RGB24   v4l2_fourcc('R','G','B','3') 
#define V4L2_PIX_FMT_BGR32   v4l2_fourcc('B','G','R','4') 
#define V4L2_PIX_FMT_RGB32   v4l2_fourcc('R','G','B','4') 
#define V4L2_PIX_FMT_GREY    v4l2_fourcc('G','R','E','Y') 
#define V4L2_PIX_FMT_PAL8    v4l2_fourcc('P','A','L','8') 
#define V4L2_PIX_FMT_YVU410  v4l2_fourcc('Y','V','U','9') 
#define V4L2_PIX_FMT_YVU420  v4l2_fourcc('Y','V','1','2') 
#define V4L2_PIX_FMT_YUYV    v4l2_fourcc('Y','U','Y','V') 
#define V4L2_PIX_FMT_UYVY    v4l2_fourcc('U','Y','V','Y') 
#define V4L2_PIX_FMT_YUV422P v4l2_fourcc('4','2','2','P') 
#define V4L2_PIX_FMT_YUV411P v4l2_fourcc('4','1','1','P') 
#define V4L2_PIX_FMT_Y41P    v4l2_fourcc('Y','4','1','P') 
#define V4L2_PIX_FMT_YUV444  v4l2_fourcc('Y','4','4','4') 
#define V4L2_PIX_FMT_YUV555  v4l2_fourcc('Y','U','V','O') 
#define V4L2_PIX_FMT_YUV565  v4l2_fourcc('Y','U','V','P') 
#define V4L2_PIX_FMT_YUV32   v4l2_fourcc('Y','U','V','4') 

#define V4L2_PIX_FMT_NV12    v4l2_fourcc('N','V','1','2') 
#define V4L2_PIX_FMT_NV21    v4l2_fourcc('N','V','2','1') 

#define V4L2_PIX_FMT_YUV410  v4l2_fourcc('Y','U','V','9') 
#define V4L2_PIX_FMT_YUV420  v4l2_fourcc('Y','U','1','2') 
#define V4L2_PIX_FMT_YYUV    v4l2_fourcc('Y','Y','U','V') 
#define V4L2_PIX_FMT_HI240   v4l2_fourcc('H','I','2','4') 
#define V4L2_PIX_FMT_HM12    v4l2_fourcc('H','M','1','2') 

#define V4L2_PIX_FMT_SBGGR8  v4l2_fourcc('B', 'A', '8', '1') 
#define V4L2_PIX_FMT_SGBRG8  v4l2_fourcc('G', 'B', 'R', 'G') 
#define V4L2_PIX_FMT_SGRBG8  v4l2_fourcc('G', 'R', 'B', 'G') 
#define V4L2_PIX_FMT_SRGGB8  v4l2_fourcc('R', 'G', 'G', 'B') 
#define V4L2_PIX_FMT_SBGGR10 v4l2_fourcc('B', 'G', '1', '\0') 
#define V4L2_PIX_FMT_SGBRG10 v4l2_fourcc('G', 'B', '1', '\0') 
#define V4L2_PIX_FMT_SGRBG10 v4l2_fourcc('B', 'A', '1', '\0') 
#define V4L2_PIX_FMT_SRGGB10 v4l2_fourcc('R', 'G', '1', '\0') 
#define V4L2_PIX_FMT_SBGGR12 v4l2_fourcc('B', 'G', '1', '2') 
#define V4L2_PIX_FMT_SGBRG12 v4l2_fourcc('G', 'B', '1', '2') 
#define V4L2_PIX_FMT_SGRBG12 v4l2_fourcc('B', 'A', '1', '2') 
#define V4L2_PIX_FMT_SRGGB12 v4l2_fourcc('R', 'G', '1', '2') 

#endif  
