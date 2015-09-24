/*
 * Copyright (c) 2007-2011 Intel Corporation. All Rights Reserved.
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
 * IN NO EVENT SHALL INTEL AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef VA_VPP_H
#define VA_VPP_H

#ifdef __cplusplus
extern "C" {
#endif


typedef enum _VAProcFilterType {
    VAProcFilterNone = 0,
    
    VAProcFilterNoiseReduction,
    
    VAProcFilterDeinterlacing,
    
    VAProcFilterSharpening,
    
    VAProcFilterColorBalance,
    VAProcFilterCount
} VAProcFilterType;

typedef enum _VAProcDeinterlacingType {
    VAProcDeinterlacingNone = 0,
    
    VAProcDeinterlacingBob,
    
    VAProcDeinterlacingWeave,
    
    VAProcDeinterlacingMotionAdaptive,
    
    VAProcDeinterlacingMotionCompensated,
    
    VAProcDeinterlacingCount
} VAProcDeinterlacingType;

typedef enum _VAProcColorBalanceType {
    VAProcColorBalanceNone = 0,
    
    VAProcColorBalanceHue,
    
    VAProcColorBalanceSaturation,
    
    VAProcColorBalanceBrightness,
    
    VAProcColorBalanceContrast,
    
    VAProcColorBalanceAutoSaturation,
    
    VAProcColorBalanceAutoBrightness,
    
    VAProcColorBalanceAutoContrast,
    
    VAProcColorBalanceCount
} VAProcColorBalanceType;

typedef enum _VAProcColorStandardType {
    VAProcColorStandardNone = 0,
    
    VAProcColorStandardBT601,
    
    VAProcColorStandardBT709,
    
    VAProcColorStandardBT470M,
    
    VAProcColorStandardBT470BG,
    
    VAProcColorStandardSMPTE170M,
    
    VAProcColorStandardSMPTE240M,
    
    VAProcColorStandardGenericFilm,
    
    VAProcColorStandardCount
} VAProcColorStandardType;

#define VA_PROC_PIPELINE_SUBPICTURES    0x00000001
#define VA_PROC_PIPELINE_FAST           0x00000002

#define VA_PROC_FILTER_MANDATORY        0x00000001

#define VA_PIPELINE_FLAG_END		0x00000004

typedef struct _VAProcPipelineCaps {
    
    unsigned int        pipeline_flags;
    
    unsigned int        filter_flags;
    
    unsigned int        num_forward_references;
    
    unsigned int        num_backward_references;
    
    VAProcColorStandardType *input_color_standards;
    
    unsigned int        num_input_color_standards;
    
    VAProcColorStandardType *output_color_standards;
    
    unsigned int        num_output_color_standards;
} VAProcPipelineCaps;

typedef struct _VAProcFilterValueRange {
    
    float               min_value;
    
    float               max_value;
    
    float               default_value;
    
    float               step;
} VAProcFilterValueRange;

typedef struct _VAProcPipelineParameterBuffer {
    VASurfaceID         surface;
    const VARectangle  *surface_region;
    VAProcColorStandardType surface_color_standard;
    const VARectangle  *output_region;
    unsigned int        output_background_color;
    VAProcColorStandardType output_color_standard;
    unsigned int        pipeline_flags;
    unsigned int        filter_flags;
    VABufferID         *filters;
    
    unsigned int        num_filters;
    
    VASurfaceID        *forward_references;
    
    unsigned int        num_forward_references;
    
    VASurfaceID        *backward_references;
    
    unsigned int        num_backward_references;
} VAProcPipelineParameterBuffer;

typedef struct _VAProcFilterParameterBufferBase {
    
    VAProcFilterType    type;
} VAProcFilterParameterBufferBase;

typedef struct _VAProcFilterParameterBuffer {
    
    VAProcFilterType    type;
    
    float               value;
} VAProcFilterParameterBuffer;

#define VA_DEINTERLACING_BOTTOM_FIELD_FIRST	0x0001
#define VA_DEINTERLACING_BOTTOM_FIELD		0x0002
#define VA_DEINTERLACING_ONE_FIELD		0x0004

typedef struct _VAProcFilterParameterBufferDeinterlacing {
    
    VAProcFilterType            type;
    
    VAProcDeinterlacingType     algorithm;
    
    unsigned int     		flags;
} VAProcFilterParameterBufferDeinterlacing;

typedef struct _VAProcFilterParameterBufferColorBalance {
    
    VAProcFilterType            type;
    
    VAProcColorBalanceType      attrib;
    float                       value;
} VAProcFilterParameterBufferColorBalance;

typedef struct _VAProcFilterCap {
    
    VAProcFilterValueRange      range;
} VAProcFilterCap;

typedef struct _VAProcFilterCapDeinterlacing {
    
    VAProcDeinterlacingType     type;
} VAProcFilterCapDeinterlacing;

typedef struct _VAProcFilterCapColorBalance {
    
    VAProcColorBalanceType      type;
    
    VAProcFilterValueRange      range;
} VAProcFilterCapColorBalance;

/**
 * \brief Queries video processing filters.
 *
 * This function returns the list of video processing filters supported
 * by the driver. The \c filters array is allocated by the user and
 * \c num_filters shall be initialized to the number of allocated
 * elements in that array. Upon successful return, the actual number
 * of filters will be overwritten into \c num_filters. Otherwise,
 * \c VA_STATUS_ERROR_MAX_NUM_EXCEEDED is returned and \c num_filters
 * is adjusted to the number of elements that would be returned if enough
 * space was available.
 *
 * The list of video processing filters supported by the driver shall
 * be ordered in the way they can be iteratively applied. This is needed
 * for both correctness, i.e. some filters would not mean anything if
 * applied at the beginning of the pipeline; but also for performance
 * since some filters can be applied in a single pass (e.g. noise
 * reduction + deinterlacing).
 *
 * @param[in] dpy               the VA display
 * @param[in] context           the video processing context
 * @param[out] filters          the output array of #VAProcFilterType elements
 * @param[in,out] num_filters the number of elements allocated on input,
 *      the number of elements actually filled in on output
 */
VAStatus
vaQueryVideoProcFilters(
    VADisplay           dpy,
    VAContextID         context,
    VAProcFilterType   *filters,
    unsigned int       *num_filters
);

/**
 * \brief Queries video filter capabilities.
 *
 * This function returns the list of capabilities supported by the driver
 * for a specific video filter. The \c filter_caps array is allocated by
 * the user and \c num_filter_caps shall be initialized to the number
 * of allocated elements in that array. Upon successful return, the
 * actual number of filters will be overwritten into \c num_filter_caps.
 * Otherwise, \c VA_STATUS_ERROR_MAX_NUM_EXCEEDED is returned and
 * \c num_filter_caps is adjusted to the number of elements that would be
 * returned if enough space was available.
 *
 * @param[in] dpy               the VA display
 * @param[in] context           the video processing context
 * @param[in] type              the video filter type
 * @param[out] filter_caps      the output array of #VAProcFilterCap elements
 * @param[in,out] num_filter_caps the number of elements allocated on input,
 *      the number of elements actually filled in output
 */
VAStatus
vaQueryVideoProcFilterCaps(
    VADisplay           dpy,
    VAContextID         context,
    VAProcFilterType    type,
    void               *filter_caps,
    unsigned int       *num_filter_caps
);

/**
 * \brief Queries video processing pipeline capabilities.
 *
 * This function returns the video processing pipeline capabilities. The
 * \c filters array defines the video processing pipeline and is an array
 * of buffers holding filter parameters.
 *
 * Note: the #VAProcPipelineCaps structure contains user-provided arrays.
 * If non-NULL, the corresponding \c num_* fields shall be filled in on
 * input with the number of elements allocated. Upon successful return,
 * the actual number of elements will be overwritten into the \c num_*
 * fields. Otherwise, \c VA_STATUS_ERROR_MAX_NUM_EXCEEDED is returned
 * and \c num_* fields are adjusted to the number of elements that would
 * be returned if enough space was available.
 *
 * @param[in] dpy               the VA display
 * @param[in] context           the video processing context
 * @param[in] filters           the array of VA buffers defining the video
 *      processing pipeline
 * @param[in] num_filters       the number of elements in filters
 * @param[in,out] pipeline_caps the video processing pipeline capabilities
 */
VAStatus
vaQueryVideoProcPipelineCaps(
    VADisplay           dpy,
    VAContextID         context,
    VABufferID         *filters,
    unsigned int        num_filters,
    VAProcPipelineCaps *pipeline_caps
);


#ifdef __cplusplus
}
#endif

#endif 
