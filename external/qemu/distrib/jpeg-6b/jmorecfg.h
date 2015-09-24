/*
 * jmorecfg.h
 *
 * Copyright (C) 1991-1997, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains additional configuration options that customize the
 * JPEG software for special applications or support machine-dependent
 * optimizations.  Most users will not need to touch this file.
 */


#include <inttypes.h>

#define ANDROID_RGB

#ifdef ANDROID_RGB
#define PACK_SHORT_565(r,g,b)  ((((r)<<8)&0xf800)|(((g)<<3)&0x7E0)|((b)>>3))
#define PACK_TWO_PIXELS(l,r)   ((r<<16) | l)
#define PACK_NEED_ALIGNMENT(ptr) (((int)(intptr_t)(ptr))&3)
#define WRITE_TWO_PIXELS(addr, pixels) do {     \
         ((INT16*)(addr))[0] = (pixels);        \
         ((INT16*)(addr))[1] = (pixels)>>16;    \
    } while(0)
#define WRITE_TWO_ALIGNED_PIXELS(addr, pixels)  ((*(INT32*)(addr)) = pixels)
#define DITHER_565_R(r, dither) ((r) + ((dither)&0xFF))
#define DITHER_565_G(g, dither) ((g) + (((dither)&0xFF)>>1))
#define DITHER_565_B(b, dither) ((b) + ((dither)&0xFF))
#endif


#define BITS_IN_JSAMPLE  8	



#define MAX_COMPONENTS  10	




#if BITS_IN_JSAMPLE == 8

#ifdef HAVE_UNSIGNED_CHAR

typedef unsigned char JSAMPLE;
#define GETJSAMPLE(value)  ((int) (value))

#else 

typedef char JSAMPLE;
#ifdef CHAR_IS_UNSIGNED
#define GETJSAMPLE(value)  ((int) (value))
#else
#define GETJSAMPLE(value)  ((int) (value) & 0xFF)
#endif 

#endif 

#define MAXJSAMPLE	255
#define CENTERJSAMPLE	128

#endif 


#if BITS_IN_JSAMPLE == 12

typedef short JSAMPLE;
#define GETJSAMPLE(value)  ((int) (value))

#define MAXJSAMPLE	4095
#define CENTERJSAMPLE	2048

#endif 



typedef short JCOEF;


/* Compressed datastreams are represented as arrays of JOCTET.
 * These must be EXACTLY 8 bits wide, at least once they are written to
 * external storage.  Note that when using the stdio data source/destination
 * managers, this is also the data type passed to fread/fwrite.
 */

#ifdef HAVE_UNSIGNED_CHAR

typedef unsigned char JOCTET;
#define GETJOCTET(value)  (value)

#else 

typedef char JOCTET;
#ifdef CHAR_IS_UNSIGNED
#define GETJOCTET(value)  (value)
#else
#define GETJOCTET(value)  ((value) & 0xFF)
#endif 

#endif 




#ifdef HAVE_UNSIGNED_CHAR
typedef unsigned char UINT8;
#else 
#ifdef CHAR_IS_UNSIGNED
typedef char UINT8;
#else 
typedef short UINT8;
#endif 
#endif 


#ifdef HAVE_UNSIGNED_SHORT
typedef unsigned short UINT16;
#else 
typedef unsigned int UINT16;
#endif 


#ifndef XMD_H			
typedef short INT16;
#endif


#ifndef XMD_H			
typedef long INT32;
#endif


typedef unsigned int JDIMENSION;

#define JPEG_MAX_DIMENSION  65500L  



#define METHODDEF(type)		static type
#define LOCAL(type)		static type
#define GLOBAL(type)		type
#define EXTERN(type)		extern type



#ifdef HAVE_PROTOTYPES
#define JMETHOD(type,methodname,arglist)  type (*methodname) arglist
#else
#define JMETHOD(type,methodname,arglist)  type (*methodname) ()
#endif



#ifdef NEED_FAR_POINTERS
#define FAR  far
#else
#define FAR
#endif



#ifndef HAVE_BOOLEAN
typedef int boolean;
#endif
#ifndef FALSE			
#define FALSE	0		
#endif
#ifndef TRUE
#define TRUE	1
#endif



#ifdef JPEG_INTERNALS
#define JPEG_INTERNAL_OPTIONS
#endif

#ifdef JPEG_INTERNAL_OPTIONS





#define DCT_ISLOW_SUPPORTED	
#define DCT_IFAST_SUPPORTED	
#define DCT_FLOAT_SUPPORTED	


#undef  C_ARITH_CODING_SUPPORTED    
#define C_MULTISCAN_FILES_SUPPORTED 
#define C_PROGRESSIVE_SUPPORTED	    
#define ENTROPY_OPT_SUPPORTED	    
#define INPUT_SMOOTHING_SUPPORTED   


#undef  D_ARITH_CODING_SUPPORTED    
#define D_MULTISCAN_FILES_SUPPORTED 
#define D_PROGRESSIVE_SUPPORTED	    
#define SAVE_MARKERS_SUPPORTED	    
#define BLOCK_SMOOTHING_SUPPORTED   
#define IDCT_SCALING_SUPPORTED	    
#undef  UPSAMPLE_SCALING_SUPPORTED  
#define UPSAMPLE_MERGING_SUPPORTED  
#define QUANT_1PASS_SUPPORTED	    
#define QUANT_2PASS_SUPPORTED	    




#define RGB_RED		0	
#define RGB_GREEN	1	
#define RGB_BLUE	2	
#ifdef ANDROID_RGB
#define RGB_ALPHA   3   
#endif
#define RGB_PIXELSIZE   3   




#ifndef INLINE
#ifdef __GNUC__			
#define INLINE __inline__
#endif
#ifndef INLINE
#define INLINE			
#endif
#endif



#ifndef MULTIPLIER
#ifdef ANDROID_INTELSSE2_IDCT
  #define MULTIPLIER short
#else
#define MULTIPLIER  int		
#endif
#endif



#ifndef FAST_FLOAT
#ifdef HAVE_PROTOTYPES
#define FAST_FLOAT  float
#else
#define FAST_FLOAT  double
#endif
#endif

#endif 
