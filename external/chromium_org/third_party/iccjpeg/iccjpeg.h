/*
 * iccprofile.h
 *
 * This file provides code to read and write International Color Consortium
 * (ICC) device profiles embedded in JFIF JPEG image files.  The ICC has
 * defined a standard format for including such data in JPEG "APP2" markers.
 * The code given here does not know anything about the internal structure
 * of the ICC profile data; it just knows how to put the profile data into
 * a JPEG file being written, or get it back out when reading.
 *
 * This code depends on new features added to the IJG JPEG library as of
 * IJG release 6b; it will not compile or work with older IJG versions.
 *
 * NOTE: this code would need surgery to work on 16-bit-int machines
 * with ICC profiles exceeding 64K bytes in size.  See iccprofile.c
 * for details.
 */

#include <stdio.h>		

#if defined(USE_SYSTEM_LIBJPEG)
#include <jpeglib.h>
#else
#include "jpeglib.h"
#endif



extern void write_icc_profile JPP((j_compress_ptr cinfo,
				   const JOCTET *icc_data_ptr,
				   unsigned int icc_data_len));





extern void setup_read_icc_profile JPP((j_decompress_ptr cinfo));



extern boolean read_icc_profile JPP((j_decompress_ptr cinfo,
				     JOCTET **icc_data_ptr,
				     unsigned int *icc_data_len));
