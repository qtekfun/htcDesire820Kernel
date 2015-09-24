// Copyright 2012 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#ifndef WEBP_WEBP_FORMAT_CONSTANTS_H_
#define WEBP_WEBP_FORMAT_CONSTANTS_H_

#define MKFOURCC(a, b, c, d) ((uint32_t)(a) | (b) << 8 | (c) << 16 | (d) << 24)

#define VP8_SIGNATURE 0x9d012a              
#define VP8_MAX_PARTITION0_SIZE (1 << 19)   
#define VP8_MAX_PARTITION_SIZE  (1 << 24)   
#define VP8_FRAME_HEADER_SIZE 10  

#define VP8L_SIGNATURE_SIZE          1      
#define VP8L_MAGIC_BYTE              0x2f   
#define VP8L_IMAGE_SIZE_BITS         14     
                                            
#define VP8L_VERSION_BITS            3      
#define VP8L_VERSION                 0      
#define VP8L_FRAME_HEADER_SIZE       5      

#define MAX_PALETTE_SIZE             256
#define MAX_CACHE_BITS               11
#define HUFFMAN_CODES_PER_META_CODE  5
#define ARGB_BLACK                   0xff000000

#define DEFAULT_CODE_LENGTH          8
#define MAX_ALLOWED_CODE_LENGTH      15

#define NUM_LITERAL_CODES            256
#define NUM_LENGTH_CODES             24
#define NUM_DISTANCE_CODES           40
#define CODE_LENGTH_CODES            19

#define MIN_HUFFMAN_BITS             2  
#define MAX_HUFFMAN_BITS             9  

#define TRANSFORM_PRESENT            1  // The bit to be written when next data
                                        
#define NUM_TRANSFORMS               4  
                                        
typedef enum {
  PREDICTOR_TRANSFORM      = 0,
  CROSS_COLOR_TRANSFORM    = 1,
  SUBTRACT_GREEN           = 2,
  COLOR_INDEXING_TRANSFORM = 3
} VP8LImageTransformType;

#define ALPHA_HEADER_LEN            1
#define ALPHA_NO_COMPRESSION        0
#define ALPHA_LOSSLESS_COMPRESSION  1
#define ALPHA_PREPROCESSED_LEVELS   1

#define TAG_SIZE           4     
#define CHUNK_SIZE_BYTES   4     
#define CHUNK_HEADER_SIZE  8     
#define RIFF_HEADER_SIZE   12    
#define ANMF_CHUNK_SIZE    16    
#define ANIM_CHUNK_SIZE    6     
#define FRGM_CHUNK_SIZE    6     
#define VP8X_CHUNK_SIZE    10    

#define MAX_CANVAS_SIZE     (1 << 24)     
#define MAX_IMAGE_AREA      (1ULL << 32)  
#define MAX_LOOP_COUNT      (1 << 16)     
#define MAX_DURATION        (1 << 24)     
#define MAX_POSITION_OFFSET (1 << 24)     

#define MAX_CHUNK_PAYLOAD (~0U - CHUNK_HEADER_SIZE - 1)

#endif  
