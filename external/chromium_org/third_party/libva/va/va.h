/*
 * Copyright (c) 2007-2009 Intel Corporation. All Rights Reserved.
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


#ifndef _VA_H_
#define _VA_H_

#include <va/va_version.h>

#ifdef __cplusplus
extern "C" {
#endif




typedef void* VADisplay;	

typedef int VAStatus;	
#define VA_STATUS_SUCCESS			0x00000000
#define VA_STATUS_ERROR_OPERATION_FAILED	0x00000001
#define VA_STATUS_ERROR_ALLOCATION_FAILED	0x00000002
#define VA_STATUS_ERROR_INVALID_DISPLAY		0x00000003
#define VA_STATUS_ERROR_INVALID_CONFIG		0x00000004
#define VA_STATUS_ERROR_INVALID_CONTEXT		0x00000005
#define VA_STATUS_ERROR_INVALID_SURFACE		0x00000006
#define VA_STATUS_ERROR_INVALID_BUFFER		0x00000007
#define VA_STATUS_ERROR_INVALID_IMAGE		0x00000008
#define VA_STATUS_ERROR_INVALID_SUBPICTURE	0x00000009
#define VA_STATUS_ERROR_ATTR_NOT_SUPPORTED	0x0000000a
#define VA_STATUS_ERROR_MAX_NUM_EXCEEDED	0x0000000b
#define VA_STATUS_ERROR_UNSUPPORTED_PROFILE	0x0000000c
#define VA_STATUS_ERROR_UNSUPPORTED_ENTRYPOINT	0x0000000d
#define VA_STATUS_ERROR_UNSUPPORTED_RT_FORMAT	0x0000000e
#define VA_STATUS_ERROR_UNSUPPORTED_BUFFERTYPE	0x0000000f
#define VA_STATUS_ERROR_SURFACE_BUSY		0x00000010
#define VA_STATUS_ERROR_FLAG_NOT_SUPPORTED      0x00000011
#define VA_STATUS_ERROR_INVALID_PARAMETER	0x00000012
#define VA_STATUS_ERROR_RESOLUTION_NOT_SUPPORTED 0x00000013
#define VA_STATUS_ERROR_UNIMPLEMENTED           0x00000014
#define VA_STATUS_ERROR_SURFACE_IN_DISPLAYING   0x00000015
#define VA_STATUS_ERROR_INVALID_IMAGE_FORMAT    0x00000016
#define VA_STATUS_ERROR_DECODING_ERROR          0x00000017
#define VA_STATUS_ERROR_ENCODING_ERROR          0x00000018
#define VA_STATUS_ERROR_INVALID_VALUE           0x00000019
#define VA_STATUS_ERROR_UNSUPPORTED_FILTER      0x00000020
#define VA_STATUS_ERROR_INVALID_FILTER_CHAIN    0x00000021
#define VA_STATUS_ERROR_HW_BUSY                 0x00000022
#define VA_STATUS_ERROR_UNKNOWN			0xFFFFFFFF

#define VA_FRAME_PICTURE        0x00000000 
#define VA_TOP_FIELD            0x00000001
#define VA_BOTTOM_FIELD         0x00000002

#define VA_ENABLE_BLEND         0x00000004  
    
#define VA_CLEAR_DRAWABLE       0x00000008

#define VA_SRC_BT601            0x00000010
#define VA_SRC_BT709            0x00000020
#define VA_SRC_SMPTE_240        0x00000040

#define VA_FILTER_SCALING_DEFAULT       0x00000000
#define VA_FILTER_SCALING_FAST          0x00000100
#define VA_FILTER_SCALING_HQ            0x00000200
#define VA_FILTER_SCALING_NL_ANAMORPHIC 0x00000300
#define VA_FILTER_SCALING_MASK          0x00000f00

const char *vaErrorStr(VAStatus error_status);

typedef void* VANativeDisplay;	

int vaDisplayIsValid(VADisplay dpy);
    
VAStatus vaInitialize (
    VADisplay dpy,
    int *major_version,	 
    int *minor_version 	 
);

 
VAStatus vaTerminate (
    VADisplay dpy
);

const char *vaQueryVendorString (
    VADisplay dpy
);

typedef int (*VAPrivFunc)();

 
VAPrivFunc vaGetLibFunc (
    VADisplay dpy,
    const char *func
);

typedef enum
{
    
    VAProfileNone                       = -1,
    VAProfileMPEG2Simple		= 0,
    VAProfileMPEG2Main			= 1,
    VAProfileMPEG4Simple		= 2,
    VAProfileMPEG4AdvancedSimple	= 3,
    VAProfileMPEG4Main			= 4,
    VAProfileH264Baseline		= 5,
    VAProfileH264Main			= 6,
    VAProfileH264High			= 7,
    VAProfileVC1Simple			= 8,
    VAProfileVC1Main			= 9,
    VAProfileVC1Advanced		= 10,
    VAProfileH263Baseline		= 11,
    VAProfileJPEGBaseline               = 12,
    VAProfileH264ConstrainedBaseline = 13
} VAProfile;

typedef enum
{
    VAEntrypointVLD		= 1,
    VAEntrypointIZZ		= 2,
    VAEntrypointIDCT		= 3,
    VAEntrypointMoComp		= 4,
    VAEntrypointDeblocking	= 5,
    VAEntrypointEncSlice	= 6,	
    VAEntrypointEncPicture 	= 7,	
    VAEntrypointVideoProc       = 10,   
    VAEntrypointMax
} VAEntrypoint;

typedef enum
{
    VAConfigAttribRTFormat		= 0,
    VAConfigAttribSpatialResidual	= 1,
    VAConfigAttribSpatialClipping	= 2,
    VAConfigAttribIntraResidual		= 3,
    VAConfigAttribEncryption		= 4,
    VAConfigAttribRateControl		= 5,

    
    
    VAConfigAttribEncPackedHeaders      = 10,
    VAConfigAttribEncInterlaced         = 11,
    VAConfigAttribEncMaxRefFrames       = 13,
    VAConfigAttribEncMaxSlices          = 14,
    VAConfigAttribEncSliceStructure     = 15,
    VAConfigAttribEncMacroblockInfo     = 16,
    
    VAConfigAttribTypeMax
} VAConfigAttribType;

typedef struct _VAConfigAttrib {
    VAConfigAttribType type;
    unsigned int value; 
} VAConfigAttrib;

#define VA_RT_FORMAT_YUV420	0x00000001	
#define VA_RT_FORMAT_YUV422	0x00000002
#define VA_RT_FORMAT_YUV444	0x00000004
#define VA_RT_FORMAT_YUV411	0x00000008
#define VA_RT_FORMAT_YUV400	0x00000010
#define VA_RT_FORMAT_RGB16	0x00010000
#define VA_RT_FORMAT_RGB32	0x00020000
 
#define VA_RT_FORMAT_RGBP	0x00100000
#define VA_RT_FORMAT_PROTECTED	0x80000000

#define VA_RC_NONE                      0x00000001
#define VA_RC_CBR                       0x00000002
#define VA_RC_VBR                       0x00000004
#define VA_RC_VCM                       0x00000008
#define VA_RC_CQP                       0x00000010
#define VA_RC_VBR_CONSTRAINED           0x00000020

#define VA_ENC_PACKED_HEADER_NONE       0x00000000
#define VA_ENC_PACKED_HEADER_SEQUENCE   0x00000001
#define VA_ENC_PACKED_HEADER_PICTURE    0x00000002
#define VA_ENC_PACKED_HEADER_SLICE      0x00000004
#define VA_ENC_PACKED_HEADER_MISC       0x00000008
#define VA_ENC_PACKED_HEADER_RAW_DATA   0x0000000C

#define VA_ENC_INTERLACED_NONE          0x00000000
#define VA_ENC_INTERLACED_FRAME         0x00000001
#define VA_ENC_INTERLACED_FIELD         0x00000002
#define VA_ENC_INTERLACED_MBAFF         0x00000004
#define VA_ENC_INTERLACED_PAFF          0x00000008

#define VA_ENC_SLICE_STRUCTURE_ARBITRARY_ROWS           0x00000000
#define VA_ENC_SLICE_STRUCTURE_POWER_OF_TWO_ROWS        0x00000001
#define VA_ENC_SLICE_STRUCTURE_ARBITRARY_MACROBLOCKS    0x00000002

#define VA_ATTRIB_NOT_SUPPORTED 0x80000000

int vaMaxNumProfiles (
    VADisplay dpy
);

int vaMaxNumEntrypoints (
    VADisplay dpy
);

int vaMaxNumConfigAttributes (
    VADisplay dpy
);

VAStatus vaQueryConfigProfiles (
    VADisplay dpy,
    VAProfile *profile_list,	
    int *num_profiles		
);

VAStatus vaQueryConfigEntrypoints (
    VADisplay dpy,
    VAProfile profile,
    VAEntrypoint *entrypoint_list,	
    int *num_entrypoints		
);

VAStatus vaGetConfigAttributes (
    VADisplay dpy,
    VAProfile profile,
    VAEntrypoint entrypoint,
    VAConfigAttrib *attrib_list, 
    int num_attribs
);

typedef unsigned int VAGenericID;

typedef VAGenericID VAConfigID;

VAStatus vaCreateConfig (
    VADisplay dpy,
    VAProfile profile, 
    VAEntrypoint entrypoint, 
    VAConfigAttrib *attrib_list,
    int num_attribs,
    VAConfigID *config_id 
);

VAStatus vaDestroyConfig (
    VADisplay dpy,
    VAConfigID config_id
);

VAStatus vaQueryConfigAttributes (
    VADisplay dpy,
    VAConfigID config_id, 
    VAProfile *profile, 	
    VAEntrypoint *entrypoint, 	
    VAConfigAttrib *attrib_list,
    int *num_attribs 		
);



typedef VAGenericID VAContextID;

typedef VAGenericID VASurfaceID;

#define VA_INVALID_ID		0xffffffff
#define VA_INVALID_SURFACE	VA_INVALID_ID

typedef enum  {
    VAGenericValueTypeInteger = 1,      
    VAGenericValueTypeFloat,            
    VAGenericValueTypePointer,          
    VAGenericValueTypeFunc              
} VAGenericValueType;

typedef void (*VAGenericFunc)(void);

typedef struct _VAGenericValue {
    
    VAGenericValueType  type;
    
    union {
        
        int             i;
        
        float           f;
        
        void           *p;
        
        VAGenericFunc   fn;
    }                   value;
} VAGenericValue;

#define VA_SURFACE_ATTRIB_NOT_SUPPORTED 0x00000000
#define VA_SURFACE_ATTRIB_GETTABLE      0x00000001
#define VA_SURFACE_ATTRIB_SETTABLE      0x00000002

typedef enum {
    VASurfaceAttribNone = 0,
    VASurfaceAttribPixelFormat,
    
    VASurfaceAttribMinWidth,
    
    VASurfaceAttribMaxWidth,
    
    VASurfaceAttribMinHeight,
    
    VASurfaceAttribMaxHeight,
    
    VASurfaceAttribMemoryType,
    
    VASurfaceAttribExternalBufferDescriptor,
    
    VASurfaceAttribCount
} VASurfaceAttribType;

typedef struct _VASurfaceAttrib {
    
    VASurfaceAttribType type;
    
    unsigned int        flags;
    
    VAGenericValue      value;
} VASurfaceAttrib;

#define VA_SURFACE_ATTRIB_MEM_TYPE_VA			0x00000001
#define VA_SURFACE_ATTRIB_MEM_TYPE_V4L2			0x00000002
#define VA_SURFACE_ATTRIB_MEM_TYPE_USER_PTR		0x00000004

typedef struct _VASurfaceAttribExternalBuffers {
    
    unsigned int pixel_format;
    
    unsigned int width;
    
    unsigned int height;
    
    unsigned int data_size;
    
    unsigned int num_planes;
    
    unsigned int pitches[4];
    
    unsigned int offsets[4];
    
    unsigned long *buffers;
    
    unsigned int num_buffers;
    
    unsigned int flags;
    
    void *private_data;
} VASurfaceAttribExternalBuffers;

#define VA_SURFACE_EXTBUF_DESC_ENABLE_TILING	0x00000001
#define VA_SURFACE_EXTBUF_DESC_CACHED		0x00000002
#define VA_SURFACE_EXTBUF_DESC_UNCACHED		0x00000004
#define VA_SURFACE_EXTBUF_DESC_WC		0x00000008
#define VA_SURFACE_EXTBUF_DESC_PROTECTED        0x80000000


/**
 * \brief Queries surface attributes for the supplied config.
 *
 * Unlike vaGetSurfaceAttributes(), this function queries for all
 * supported attributes for the supplied VA @config. In particular, if
 * the underlying hardware supports the creation of VA surfaces in
 * various formats, then this function will enumerate all pixel
 * formats that are supported.
 *
 * The \c attrib_list array is allocated by the user and \c
 * num_attribs shall be initialized to the number of allocated
 * elements in that array. Upon successful return, the actual number
 * of attributes will be overwritten into \c num_attribs. Otherwise,
 * \c VA_STATUS_ERROR_MAX_NUM_EXCEEDED is returned and \c num_attribs
 * is adjusted to the number of elements that would be returned if
 * enough space was available.
 *
 * Note: it is perfectly valid to pass NULL to the \c attrib_list
 * argument when vaQuerySurfaceAttributes() is used to determine the
 * actual number of elements that need to be allocated.
 *
 * @param[in] dpy               the VA display
 * @param[in] config            the config identifying a codec or a video
 *     processing pipeline
 * @param[out] attrib_list      the output array of #VASurfaceAttrib elements
 * @param[in,out] num_attribs   the number of elements allocated on
 *      input, the number of elements actually filled in output
 */
VAStatus
vaQuerySurfaceAttributes(
    VADisplay           dpy,
    VAConfigID          config,
    VASurfaceAttrib    *attrib_list,
    unsigned int       *num_attribs
);

VAStatus
vaCreateSurfaces(
    VADisplay           dpy,
    unsigned int        format,
    unsigned int        width,
    unsigned int        height,
    VASurfaceID        *surfaces,
    unsigned int        num_surfaces,
    VASurfaceAttrib    *attrib_list,
    unsigned int        num_attribs
);
    
VAStatus vaDestroySurfaces (
    VADisplay dpy,
    VASurfaceID *surfaces,
    int num_surfaces
);

#define VA_PROGRESSIVE 0x1
VAStatus vaCreateContext (
    VADisplay dpy,
    VAConfigID config_id,
    int picture_width,
    int picture_height,
    int flag,
    VASurfaceID *render_targets,
    int num_render_targets,
    VAContextID *context		
);

VAStatus vaDestroyContext (
    VADisplay dpy,
    VAContextID context
);


typedef VAGenericID VABufferID;

typedef enum
{
    VAPictureParameterBufferType	= 0,
    VAIQMatrixBufferType		= 1,
    VABitPlaneBufferType		= 2,
    VASliceGroupMapBufferType		= 3,
    VASliceParameterBufferType		= 4,
    VASliceDataBufferType		= 5,
    VAMacroblockParameterBufferType	= 6,
    VAResidualDataBufferType		= 7,
    VADeblockingParameterBufferType	= 8,
    VAImageBufferType			= 9,
    VAProtectedSliceDataBufferType	= 10,
    VAQMatrixBufferType                 = 11,
    VAHuffmanTableBufferType            = 12,

    VAEncCodedBufferType		= 21,
    VAEncSequenceParameterBufferType	= 22,
    VAEncPictureParameterBufferType	= 23,
    VAEncSliceParameterBufferType	= 24,
    VAEncPackedHeaderParameterBufferType = 25,
    VAEncPackedHeaderDataBufferType     = 26,
    VAEncMiscParameterBufferType	= 27,
    VAEncMacroblockParameterBufferType	= 28,
    VAProcPipelineParameterBufferType   = 41,
    VAProcFilterParameterBufferType     = 42,
    VABufferTypeMax
} VABufferType;

typedef enum
{
    VAEncMiscParameterTypeFrameRate 	= 0,
    VAEncMiscParameterTypeRateControl  	= 1,
    VAEncMiscParameterTypeMaxSliceSize	= 2,
    VAEncMiscParameterTypeAIR    	= 3,
    
    VAEncMiscParameterTypeMaxFrameSize  = 4,
    
    VAEncMiscParameterTypeHRD           = 5,
} VAEncMiscParameterType;

typedef enum {
    
    VAEncPackedHeaderSequence   = 1,
    
    VAEncPackedHeaderPicture    = 2,
    
    VAEncPackedHeaderSlice      = 3,
    VAEncPackedHeaderRawData    = 4,
    
    VAEncPackedHeaderMiscMask   = 0x80000000,
} VAEncPackedHeaderType;

typedef struct _VAEncPackedHeaderParameterBuffer {
    
    unsigned int                type;
    
    unsigned int                bit_length;
    
    unsigned char               has_emulation_bytes;
} VAEncPackedHeaderParameterBuffer;

typedef struct _VAEncMiscParameterBuffer
{
    VAEncMiscParameterType type;
    unsigned int data[0];
} VAEncMiscParameterBuffer;


typedef struct _VAEncMiscParameterRateControl
{
    
    unsigned int bits_per_second;
    unsigned int target_percentage;
    unsigned int window_size;
    
    unsigned int initial_qp;
    unsigned int min_qp;
    unsigned int basic_unit_size;
    union
    {
        struct
        {
            unsigned int reset : 1;
            unsigned int disable_frame_skip : 1; 
            unsigned int disable_bit_stuffing : 1; 
        } bits;
        unsigned int value;
    } rc_flags;
} VAEncMiscParameterRateControl;

typedef struct _VAEncMiscParameterFrameRate
{
    unsigned int framerate;
} VAEncMiscParameterFrameRate;

typedef struct _VAEncMiscParameterMaxSliceSize
{
    unsigned int max_slice_size;
} VAEncMiscParameterMaxSliceSize;

typedef struct _VAEncMiscParameterAIR
{
    unsigned int air_num_mbs;
    unsigned int air_threshold;
    unsigned int air_auto; 
} VAEncMiscParameterAIR;

typedef struct _VAEncMiscParameterHRD
{
    unsigned int initial_buffer_fullness;       
    unsigned int buffer_size;                   
} VAEncMiscParameterHRD;

typedef struct _VAEncMiscParameterBufferMaxFrameSize {
    
    VAEncMiscParameterType      type;
    
    unsigned int                max_frame_size;
} VAEncMiscParameterBufferMaxFrameSize;

#define VA_SLICE_DATA_FLAG_ALL		0x00	
#define VA_SLICE_DATA_FLAG_BEGIN	0x01	
#define VA_SLICE_DATA_FLAG_MIDDLE	0x02	
#define VA_SLICE_DATA_FLAG_END		0x04	

typedef struct _VASliceParameterBufferBase
{
    unsigned int slice_data_size;	
    unsigned int slice_data_offset;	
    unsigned int slice_data_flag;	
} VASliceParameterBufferBase;


typedef struct _VAQMatrixBufferJPEG
{
    int load_lum_quantiser_matrix;
    int load_chroma_quantiser_matrix;
    unsigned char lum_quantiser_matrix[64];
    unsigned char chroma_quantiser_matrix[64];
} VAQMatrixBufferJPEG;

typedef struct _VAEncPictureParameterBufferJPEG
{
    VASurfaceID reconstructed_picture;
    unsigned short picture_width;
    unsigned short picture_height;
    VABufferID coded_buf;
} VAEncPictureParameterBufferJPEG;

#include <va/va_dec_jpeg.h>

 
typedef struct _VAPictureParameterBufferMPEG2
{
    unsigned short horizontal_size;
    unsigned short vertical_size;
    VASurfaceID forward_reference_picture;
    VASurfaceID backward_reference_picture;
    
    int picture_coding_type;
    int f_code; 
    union {
        struct {
            unsigned int intra_dc_precision		: 2; 
            unsigned int picture_structure		: 2; 
            unsigned int top_field_first		: 1; 
            unsigned int frame_pred_frame_dct		: 1; 
            unsigned int concealment_motion_vectors	: 1;
            unsigned int q_scale_type			: 1;
            unsigned int intra_vlc_format		: 1;
            unsigned int alternate_scan			: 1;
            unsigned int repeat_first_field		: 1;
            unsigned int progressive_frame		: 1;
            unsigned int is_first_field			: 1; 
        } bits;
        unsigned int value;
    } picture_coding_extension;
} VAPictureParameterBufferMPEG2;

typedef struct _VAIQMatrixBufferMPEG2
{
    int load_intra_quantiser_matrix;
    int load_non_intra_quantiser_matrix;
    int load_chroma_intra_quantiser_matrix;
    int load_chroma_non_intra_quantiser_matrix;
    unsigned char intra_quantiser_matrix[64];
    unsigned char non_intra_quantiser_matrix[64];
    unsigned char chroma_intra_quantiser_matrix[64];
    unsigned char chroma_non_intra_quantiser_matrix[64];
} VAIQMatrixBufferMPEG2;

typedef struct _VASliceParameterBufferMPEG2
{
    unsigned int slice_data_size;
    unsigned int slice_data_offset;
    unsigned int slice_data_flag; 
    unsigned int macroblock_offset;
    unsigned int slice_horizontal_position;
    unsigned int slice_vertical_position;
    int quantiser_scale_code;
    int intra_slice_flag;
} VASliceParameterBufferMPEG2;

typedef struct _VAMacroblockParameterBufferMPEG2
{
    unsigned short macroblock_address;
    unsigned char macroblock_type;  
    union {
        struct {
            unsigned int frame_motion_type		: 2; 
            unsigned int field_motion_type		: 2; 
            unsigned int dct_type			: 1; 
        } bits;
        unsigned int value;
    } macroblock_modes;
    unsigned char motion_vertical_field_select; 
    short PMV[2][2][2]; 
    unsigned short coded_block_pattern;
     
    
    unsigned short num_skipped_macroblocks;
} VAMacroblockParameterBufferMPEG2;

#define VA_MB_TYPE_MOTION_FORWARD	0x02
#define VA_MB_TYPE_MOTION_BACKWARD	0x04
#define VA_MB_TYPE_MOTION_PATTERN	0x08
#define VA_MB_TYPE_MOTION_INTRA		0x10


 
typedef struct _VAPictureParameterBufferMPEG4
{
    unsigned short vop_width;
    unsigned short vop_height;
    VASurfaceID forward_reference_picture;
    VASurfaceID backward_reference_picture;
    union {
        struct {
            unsigned int short_video_header		: 1; 
            unsigned int chroma_format			: 2; 
            unsigned int interlaced			: 1; 
            unsigned int obmc_disable			: 1; 
            unsigned int sprite_enable			: 2; 
            unsigned int sprite_warping_accuracy	: 2; 
            unsigned int quant_type			: 1; 
            unsigned int quarter_sample			: 1; 
            unsigned int data_partitioned		: 1; 
            unsigned int reversible_vlc			: 1; 
            unsigned int resync_marker_disable		: 1; 
        } bits;
        unsigned int value;
    } vol_fields;
    unsigned char no_of_sprite_warping_points;
    short sprite_trajectory_du[3];
    short sprite_trajectory_dv[3];
    unsigned char quant_precision;
    union {
        struct {
            unsigned int vop_coding_type		: 2; 
            unsigned int backward_reference_vop_coding_type	: 2; 
            unsigned int vop_rounding_type		: 1; 
            unsigned int intra_dc_vlc_thr		: 3; 
            unsigned int top_field_first		: 1; 
            unsigned int alternate_vertical_scan_flag	: 1; 
        } bits;
        unsigned int value;
    } vop_fields;
    unsigned char vop_fcode_forward;
    unsigned char vop_fcode_backward;
    unsigned short vop_time_increment_resolution;
    
    unsigned char num_gobs_in_vop;
    unsigned char num_macroblocks_in_gob;
    
    short TRB;
    short TRD;
} VAPictureParameterBufferMPEG4;

typedef struct _VAIQMatrixBufferMPEG4
{
    int load_intra_quant_mat;
    int load_non_intra_quant_mat;
    unsigned char intra_quant_mat[64];
    unsigned char non_intra_quant_mat[64];
} VAIQMatrixBufferMPEG4;

typedef struct _VASliceParameterBufferMPEG4
{
    unsigned int slice_data_size;
    unsigned int slice_data_offset;
    unsigned int slice_data_flag; 
    unsigned int macroblock_offset;
    unsigned int macroblock_number;
    int quant_scale;
} VASliceParameterBufferMPEG4;


typedef enum   
{
    VAMvMode1Mv                        = 0,
    VAMvMode1MvHalfPel                 = 1,
    VAMvMode1MvHalfPelBilinear         = 2,
    VAMvModeMixedMv                    = 3,
    VAMvModeIntensityCompensation      = 4 
} VAMvModeVC1;

typedef struct _VAPictureParameterBufferVC1
{
    VASurfaceID forward_reference_picture;
    VASurfaceID backward_reference_picture;
    VASurfaceID inloop_decoded_picture;

    
    union {
        struct {
            unsigned int pulldown	: 1; 
            unsigned int interlace	: 1; 
            unsigned int tfcntrflag	: 1; 
            unsigned int finterpflag	: 1; 
            unsigned int psf		: 1; 
            unsigned int multires	: 1; 
            unsigned int overlap	: 1; 
            unsigned int syncmarker	: 1; 
            unsigned int rangered	: 1; 
            unsigned int max_b_frames	: 3; 
            unsigned int profile	: 2; 
        } bits;
        unsigned int value;
    } sequence_fields;

    unsigned short coded_width;		
    unsigned short coded_height;	
    union {
	struct {
            unsigned int broken_link	: 1; 
            unsigned int closed_entry	: 1; 
            unsigned int panscan_flag	: 1; 
            unsigned int loopfilter	: 1; 
	} bits;
	unsigned int value;
    } entrypoint_fields;
    unsigned char conditional_overlap_flag; 
    unsigned char fast_uvmc_flag;	
    union {
        struct {
            unsigned int luma_flag	: 1; 
            unsigned int luma		: 3; 
            unsigned int chroma_flag	: 1; 
            unsigned int chroma		: 3; 
        } bits;
        unsigned int value;
    } range_mapping_fields;

    unsigned char b_picture_fraction;	
    unsigned char cbp_table;		
    unsigned char mb_mode_table;	
    unsigned char range_reduction_frame;
    unsigned char rounding_control;	
    unsigned char post_processing;	
    unsigned char picture_resolution_index;	
    unsigned char luma_scale;		
    unsigned char luma_shift;		
    union {
        struct {
            unsigned int picture_type		: 3; 
            unsigned int frame_coding_mode	: 3; 
            unsigned int top_field_first	: 1; 
            unsigned int is_first_field		: 1; 
            unsigned int intensity_compensation	: 1; 
        } bits;
        unsigned int value;
    } picture_fields;
    union {
        struct {
            unsigned int mv_type_mb	: 1; 	
            unsigned int direct_mb	: 1; 	
            unsigned int skip_mb	: 1; 	
            unsigned int field_tx	: 1; 	
            unsigned int forward_mb	: 1;	
            unsigned int ac_pred	: 1;	
            unsigned int overflags	: 1;	
        } flags;
        unsigned int value;
    } raw_coding;
    union {
        struct {
            unsigned int bp_mv_type_mb   : 1;    
            unsigned int bp_direct_mb    : 1;    
            unsigned int bp_skip_mb      : 1;      
            unsigned int bp_field_tx     : 1;     
            unsigned int bp_forward_mb   : 1;    
            unsigned int bp_ac_pred      : 1;       
            unsigned int bp_overflags    : 1;    
        } flags;
        unsigned int value;
    } bitplane_present; 
    union {
        struct {
            unsigned int reference_distance_flag : 1;
            unsigned int reference_distance	: 5;
            unsigned int num_reference_pictures: 1;
            unsigned int reference_field_pic_indicator	: 1;
        } bits;
        unsigned int value;
    } reference_fields;
    union {
        struct {
            unsigned int mv_mode		: 3; 
            unsigned int mv_mode2		: 3; 
            unsigned int mv_table		: 3; 
            unsigned int two_mv_block_pattern_table: 2; 
            unsigned int four_mv_switch		: 1; 
            unsigned int four_mv_block_pattern_table : 2; 
            unsigned int extended_mv_flag	: 1; 
            unsigned int extended_mv_range	: 2; 
            unsigned int extended_dmv_flag	: 1; 
            unsigned int extended_dmv_range	: 2; 
        } bits;
        unsigned int value;
    } mv_fields;
    union {
        struct {
            unsigned int dquant	: 2; 	
            unsigned int quantizer     : 2; 	
            unsigned int half_qp	: 1; 	
            unsigned int pic_quantizer_scale : 5;
            unsigned int pic_quantizer_type : 1;
            unsigned int dq_frame	: 1; 	
            unsigned int dq_profile	: 2; 	
            unsigned int dq_sb_edge	: 2; 	
            unsigned int dq_db_edge 	: 2; 	
            unsigned int dq_binary_level : 1; 	
            unsigned int alt_pic_quantizer : 5;
        } bits;
        unsigned int value;
    } pic_quantizer_fields;
    union {
        struct {
            unsigned int variable_sized_transform_flag	: 1;
            unsigned int mb_level_transform_type_flag	: 1;
            unsigned int frame_level_transform_type	: 2;
            unsigned int transform_ac_codingset_idx1	: 2;
            unsigned int transform_ac_codingset_idx2	: 2;
            unsigned int intra_transform_dc_table	: 1;
        } bits;
        unsigned int value;
    } transform_fields;
} VAPictureParameterBufferVC1;


typedef struct _VASliceParameterBufferVC1
{
    unsigned int slice_data_size;
    unsigned int slice_data_offset;
    unsigned int slice_data_flag; 
    unsigned int macroblock_offset;
    unsigned int slice_vertical_position;
} VASliceParameterBufferVC1;



typedef struct _VAPictureH264
{
    VASurfaceID picture_id;
    unsigned int frame_idx;
    unsigned int flags;
    signed int TopFieldOrderCnt;
    signed int BottomFieldOrderCnt;
} VAPictureH264;
#define VA_PICTURE_H264_INVALID			0x00000001
#define VA_PICTURE_H264_TOP_FIELD		0x00000002
#define VA_PICTURE_H264_BOTTOM_FIELD		0x00000004
#define VA_PICTURE_H264_SHORT_TERM_REFERENCE	0x00000008
#define VA_PICTURE_H264_LONG_TERM_REFERENCE	0x00000010

typedef struct _VAPictureParameterBufferH264
{
    VAPictureH264 CurrPic;
    VAPictureH264 ReferenceFrames[16];	
    unsigned short picture_width_in_mbs_minus1;
    unsigned short picture_height_in_mbs_minus1;
    unsigned char bit_depth_luma_minus8;
    unsigned char bit_depth_chroma_minus8;
    unsigned char num_ref_frames;
    union {
        struct {
            unsigned int chroma_format_idc			: 2; 
            unsigned int residual_colour_transform_flag		: 1; 
            unsigned int gaps_in_frame_num_value_allowed_flag	: 1; 
            unsigned int frame_mbs_only_flag			: 1; 
            unsigned int mb_adaptive_frame_field_flag		: 1; 
            unsigned int direct_8x8_inference_flag		: 1; 
            unsigned int MinLumaBiPredSize8x8			: 1; 
            unsigned int log2_max_frame_num_minus4		: 4;
            unsigned int pic_order_cnt_type			: 2;
            unsigned int log2_max_pic_order_cnt_lsb_minus4	: 4;
            unsigned int delta_pic_order_always_zero_flag	: 1;
        } bits;
        unsigned int value;
    } seq_fields;
    unsigned char num_slice_groups_minus1;
    unsigned char slice_group_map_type;
    unsigned short slice_group_change_rate_minus1;
    signed char pic_init_qp_minus26;
    signed char pic_init_qs_minus26;
    signed char chroma_qp_index_offset;
    signed char second_chroma_qp_index_offset;
    union {
        struct {
            unsigned int entropy_coding_mode_flag	: 1;
            unsigned int weighted_pred_flag		: 1;
            unsigned int weighted_bipred_idc		: 2;
            unsigned int transform_8x8_mode_flag	: 1;
            unsigned int field_pic_flag			: 1;
            unsigned int constrained_intra_pred_flag	: 1;
            unsigned int pic_order_present_flag			: 1;
            unsigned int deblocking_filter_control_present_flag : 1;
            unsigned int redundant_pic_cnt_present_flag		: 1;
            unsigned int reference_pic_flag			: 1; 
        } bits;
        unsigned int value;
    } pic_fields;
    unsigned short frame_num;
} VAPictureParameterBufferH264;

typedef struct _VAIQMatrixBufferH264
{
    unsigned char ScalingList4x4[6][16];
    unsigned char ScalingList8x8[2][64];
} VAIQMatrixBufferH264;

 

typedef struct _VASliceParameterBufferH264
{
    unsigned int slice_data_size;
    
    unsigned int slice_data_offset;
    unsigned int slice_data_flag; 
    unsigned short slice_data_bit_offset;
    unsigned short first_mb_in_slice;
    unsigned char slice_type;
    unsigned char direct_spatial_mv_pred_flag;
    unsigned char num_ref_idx_l0_active_minus1;
    unsigned char num_ref_idx_l1_active_minus1;
    unsigned char cabac_init_idc;
    char slice_qp_delta;
    unsigned char disable_deblocking_filter_idc;
    char slice_alpha_c0_offset_div2;
    char slice_beta_offset_div2;
    VAPictureH264 RefPicList0[32];	
    VAPictureH264 RefPicList1[32];	
    unsigned char luma_log2_weight_denom;
    unsigned char chroma_log2_weight_denom;
    unsigned char luma_weight_l0_flag;
    short luma_weight_l0[32];
    short luma_offset_l0[32];
    unsigned char chroma_weight_l0_flag;
    short chroma_weight_l0[32][2];
    short chroma_offset_l0[32][2];
    unsigned char luma_weight_l1_flag;
    short luma_weight_l1[32];
    short luma_offset_l1[32];
    unsigned char chroma_weight_l1_flag;
    short chroma_weight_l1[32][2];
    short chroma_offset_l1[32][2];
} VASliceParameterBufferH264;

typedef enum
{
    VAEncPictureTypeIntra		= 0,
    VAEncPictureTypePredictive		= 1,
    VAEncPictureTypeBidirectional	= 2,
} VAEncPictureType;

typedef struct _VAEncSliceParameterBuffer
{
    unsigned int start_row_number;	
    unsigned int slice_height;	
    union {
        struct {
            unsigned int is_intra	: 1;
            unsigned int disable_deblocking_filter_idc : 2;
            unsigned int uses_long_term_ref		:1;
            unsigned int is_long_term_ref		:1;
        } bits;
        unsigned int value;
    } slice_flags;
} VAEncSliceParameterBuffer;



typedef struct _VAEncSequenceParameterBufferH263
{
    unsigned int intra_period;
    unsigned int bits_per_second;
    unsigned int frame_rate;
    unsigned int initial_qp;
    unsigned int min_qp;
} VAEncSequenceParameterBufferH263;

typedef struct _VAEncPictureParameterBufferH263
{
    VASurfaceID reference_picture;
    VASurfaceID reconstructed_picture;
    VABufferID coded_buf;
    unsigned short picture_width;
    unsigned short picture_height;
    VAEncPictureType picture_type;
} VAEncPictureParameterBufferH263;


typedef struct _VAEncSequenceParameterBufferMPEG4
{
    unsigned char profile_and_level_indication;
    unsigned int intra_period;
    unsigned int video_object_layer_width;
    unsigned int video_object_layer_height;
    unsigned int vop_time_increment_resolution;
    unsigned int fixed_vop_rate;
    unsigned int fixed_vop_time_increment;
    unsigned int bits_per_second;
    unsigned int frame_rate;
    unsigned int initial_qp;
    unsigned int min_qp;
} VAEncSequenceParameterBufferMPEG4;

typedef struct _VAEncPictureParameterBufferMPEG4
{
    VASurfaceID reference_picture;
    VASurfaceID reconstructed_picture;
    VABufferID coded_buf;
    unsigned short picture_width;
    unsigned short picture_height;
    unsigned int modulo_time_base; 
    unsigned int vop_time_increment;
    VAEncPictureType picture_type;
} VAEncPictureParameterBufferMPEG4;




VAStatus vaCreateBuffer (
    VADisplay dpy,
    VAContextID context,
    VABufferType type,	
    unsigned int size,	
    unsigned int num_elements, 
    void *data,		
    VABufferID *buf_id	
);

VAStatus vaBufferSetNumElements (
    VADisplay dpy,
    VABufferID buf_id,	
    unsigned int num_elements 
);



#define VA_CODED_BUF_STATUS_PICTURE_AVE_QP_MASK         0xff
#define VA_CODED_BUF_STATUS_LARGE_SLICE_MASK            0x100
#define VA_CODED_BUF_STATUS_SLICE_OVERFLOW_MASK         0x200
#define VA_CODED_BUF_STATUS_BITRATE_OVERFLOW		0x400
#define VA_CODED_BUF_STATUS_BITRATE_HIGH		0x800
#define VA_CODED_BUF_STATUS_FRAME_SIZE_OVERFLOW         0x1000
#define VA_CODED_BUF_STATUS_AIR_MB_OVER_THRESHOLD	0xff0000

#define VA_CODED_BUF_STATUS_SINGLE_NALU                 0x10000000	

typedef  struct _VACodedBufferSegment  {
    unsigned int        size;
    
    unsigned int        bit_offset;
    
    unsigned int        status;
    
    unsigned int        reserved;
    
    void               *buf;
    void               *next;
} VACodedBufferSegment;
     
VAStatus vaMapBuffer (
    VADisplay dpy,
    VABufferID buf_id,	
    void **pbuf 	
);

VAStatus vaUnmapBuffer (
    VADisplay dpy,
    VABufferID buf_id	
);

VAStatus vaDestroyBuffer (
    VADisplay dpy,
    VABufferID buffer_id
);


VAStatus vaBeginPicture (
    VADisplay dpy,
    VAContextID context,
    VASurfaceID render_target
);

VAStatus vaRenderPicture (
    VADisplay dpy,
    VAContextID context,
    VABufferID *buffers,
    int num_buffers
);

VAStatus vaEndPicture (
    VADisplay dpy,
    VAContextID context
);


VAStatus vaSyncSurface (
    VADisplay dpy,
    VASurfaceID render_target
);

typedef enum
{
    VASurfaceRendering	= 1,  
    VASurfaceDisplaying	= 2,  
                             
                             
    VASurfaceReady	= 4, 
    VASurfaceSkipped	= 8  
} VASurfaceStatus;

VAStatus vaQuerySurfaceStatus (
    VADisplay dpy,
    VASurfaceID render_target,
    VASurfaceStatus *status	
);

typedef enum
{
    VADecodeSliceMissing            = 0,
    VADecodeMBError                 = 1,
} VADecodeErrorType;

typedef struct _VASurfaceDecodeMBErrors
{
    int status; 
    unsigned int start_mb; 
    unsigned int end_mb;  
    VADecodeErrorType decode_error_type;
} VASurfaceDecodeMBErrors;

VAStatus vaQuerySurfaceError(
    VADisplay dpy,
    VASurfaceID surface,
    VAStatus error_status,
    void **error_info
);

#define VA_FOURCC(ch0, ch1, ch2, ch3) \
    ((unsigned long)(unsigned char) (ch0) | ((unsigned long)(unsigned char) (ch1) << 8) | \
    ((unsigned long)(unsigned char) (ch2) << 16) | ((unsigned long)(unsigned char) (ch3) << 24 ))

#define VA_FOURCC_NV12		0x3231564E
#define VA_FOURCC_AI44		0x34344149
#define VA_FOURCC_RGBA		0x41424752
#define VA_FOURCC_RGBX		0x58424752
#define VA_FOURCC_BGRA		0x41524742
#define VA_FOURCC_BGRX		0x58524742
#define VA_FOURCC_ARGB		0x42475241
#define VA_FOURCC_XRGB		0x42475258
#define VA_FOURCC_UYVY          0x59565955
#define VA_FOURCC_YUY2          0x32595559
#define VA_FOURCC_AYUV          0x56555941
#define VA_FOURCC_NV11          0x3131564e
#define VA_FOURCC_YV12          0x32315659
#define VA_FOURCC_P208          0x38303250
#define VA_FOURCC_IYUV          0x56555949
#define VA_FOURCC_YV24          0x34325659
#define VA_FOURCC_YV32          0x32335659
#define VA_FOURCC_Y800          0x30303859
#define VA_FOURCC_IMC3          0x33434D49
#define VA_FOURCC_411P          0x50313134
#define VA_FOURCC_422H          0x48323234
#define VA_FOURCC_422V          0x56323234
#define VA_FOURCC_444P          0x50343434
#define VA_FOURCC_RGBP          0x50424752
#define VA_FOURCC_BGRP          0x50524742
#define VA_FOURCC_411R          0x52313134 

#define VA_LSB_FIRST		1
#define VA_MSB_FIRST		2

typedef struct _VAImageFormat
{
    unsigned int	fourcc;
    unsigned int	byte_order; 
    unsigned int	bits_per_pixel;
    
    unsigned int	depth; 
    unsigned int	red_mask;
    unsigned int	green_mask;
    unsigned int	blue_mask;
    unsigned int	alpha_mask;
} VAImageFormat;

typedef VAGenericID VAImageID;

typedef struct _VAImage
{
    VAImageID		image_id; 
    VAImageFormat	format;
    VABufferID		buf;	
    unsigned short	width; 
    unsigned short	height;
    unsigned int	data_size;
    unsigned int	num_planes;	
    unsigned int	pitches[3];
    unsigned int	offsets[3];

    
    int num_palette_entries;   
    int entry_bytes; 
    char component_order[4];
} VAImage;

int vaMaxNumImageFormats (
    VADisplay dpy
);

VAStatus vaQueryImageFormats (
    VADisplay dpy,
    VAImageFormat *format_list,	
    int *num_formats		
);

VAStatus vaCreateImage (
    VADisplay dpy,
    VAImageFormat *format,
    int width,
    int height,
    VAImage *image	
);

VAStatus vaDestroyImage (
    VADisplay dpy,
    VAImageID image
);

VAStatus vaSetImagePalette (
    VADisplay dpy,
    VAImageID image,
    unsigned char *palette 
);

VAStatus vaGetImage (
    VADisplay dpy,
    VASurfaceID surface,
    int x,	
    int y,
    unsigned int width, 
    unsigned int height,
    VAImageID image
);

VAStatus vaPutImage (
    VADisplay dpy,
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

VAStatus vaDeriveImage (
    VADisplay dpy,
    VASurfaceID surface,
    VAImage *image	
);


typedef VAGenericID VASubpictureID;

int vaMaxNumSubpictureFormats (
    VADisplay dpy
);

#define VA_SUBPICTURE_CHROMA_KEYING			0x0001
#define VA_SUBPICTURE_GLOBAL_ALPHA			0x0002
#define VA_SUBPICTURE_DESTINATION_IS_SCREEN_COORD	0x0004

VAStatus vaQuerySubpictureFormats (
    VADisplay dpy,
    VAImageFormat *format_list,	
    unsigned int *flags,	
    unsigned int *num_formats	
);

VAStatus vaCreateSubpicture (
    VADisplay dpy,
    VAImageID image,
    VASubpictureID *subpicture	
);

VAStatus vaDestroySubpicture (
    VADisplay dpy,
    VASubpictureID subpicture
);

VAStatus vaSetSubpictureImage (
    VADisplay dpy,
    VASubpictureID subpicture,
    VAImageID image
);

VAStatus vaSetSubpictureChromakey (
    VADisplay dpy,
    VASubpictureID subpicture,
    unsigned int chromakey_min,
    unsigned int chromakey_max,
    unsigned int chromakey_mask
);

VAStatus vaSetSubpictureGlobalAlpha (
    VADisplay dpy,
    VASubpictureID subpicture,
    float global_alpha 
);

VAStatus vaAssociateSubpicture (
    VADisplay dpy,
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

VAStatus vaDeassociateSubpicture (
    VADisplay dpy,
    VASubpictureID subpicture,
    VASurfaceID *target_surfaces,
    int num_surfaces
);

typedef struct _VARectangle
{
    short x;
    short y;
    unsigned short width;
    unsigned short height;
} VARectangle;

typedef enum
{
    VADISPLAYATTRIB_BLE_OFF              = 0x00,
    VADISPLAYATTRIB_BLE_LOW,
    VADISPLAYATTRIB_BLE_MEDIUM,
    VADISPLAYATTRIB_BLE_HIGH,
    VADISPLAYATTRIB_BLE_NONE,
} VADisplayAttribBLEMode;

#define VA_ROTATION_NONE        0x00000000
#define VA_ROTATION_90          0x00000001
#define VA_ROTATION_180         0x00000002
#define VA_ROTATION_270         0x00000003

#define VA_OOL_DEBLOCKING_FALSE 0x00000000
#define VA_OOL_DEBLOCKING_TRUE  0x00000001

#define VA_RENDER_MODE_UNDEFINED           0
#define VA_RENDER_MODE_LOCAL_OVERLAY       1
#define VA_RENDER_MODE_LOCAL_GPU           2
#define VA_RENDER_MODE_EXTERNAL_OVERLAY    4
#define VA_RENDER_MODE_EXTERNAL_GPU        8

#define VA_RENDER_DEVICE_UNDEFINED  0
#define VA_RENDER_DEVICE_LOCAL      1
#define VA_RENDER_DEVICE_EXTERNAL   2

typedef enum
{
    VADisplayAttribBrightness		= 0,
    VADisplayAttribContrast		= 1,
    VADisplayAttribHue			= 2,
    VADisplayAttribSaturation		= 3,
    VADisplayAttribBackgroundColor      = 4,
    VADisplayAttribDirectSurface       = 5,
    VADisplayAttribRotation            = 6,	
    VADisplayAttribOutofLoopDeblock    = 7,

    
    VADisplayAttribBLEBlackMode        = 8,
    VADisplayAttribBLEWhiteMode        = 9,
    VADisplayAttribBlueStretch         = 10,
    VADisplayAttribSkinColorCorrection = 11,
    VADisplayAttribCSCMatrix           = 12,
    VADisplayAttribBlendColor          = 13,
    VADisplayAttribOverlayAutoPaintColorKey   = 14,
    VADisplayAttribOverlayColorKey	= 15,
    VADisplayAttribRenderMode           = 16,
    VADisplayAttribRenderDevice        = 17,
    VADisplayAttribRenderRect          = 18,
} VADisplayAttribType;

#define VA_DISPLAY_ATTRIB_NOT_SUPPORTED	0x0000
#define VA_DISPLAY_ATTRIB_GETTABLE	0x0001
#define VA_DISPLAY_ATTRIB_SETTABLE	0x0002

typedef struct _VADisplayAttribute
{
    VADisplayAttribType type;
    int min_value;
    int max_value;
    int value;	
    unsigned int flags;
} VADisplayAttribute;

int vaMaxNumDisplayAttributes (
    VADisplay dpy
);

VAStatus vaQueryDisplayAttributes (
    VADisplay dpy,
    VADisplayAttribute *attr_list,	
    int *num_attributes			
);

VAStatus vaGetDisplayAttributes (
    VADisplay dpy,
    VADisplayAttribute *attr_list,	
    int num_attributes
);

VAStatus vaSetDisplayAttributes (
    VADisplay dpy,
    VADisplayAttribute *attr_list,
    int num_attributes
);


#ifdef __cplusplus
}
#endif

#endif 
