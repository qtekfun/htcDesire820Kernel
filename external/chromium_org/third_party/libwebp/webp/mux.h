// Copyright 2011 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license


#ifndef WEBP_WEBP_MUX_H_
#define WEBP_WEBP_MUX_H_

#include "./mux_types.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#define WEBP_MUX_ABI_VERSION 0x0100        

typedef struct WebPMux WebPMux;   
typedef struct WebPMuxFrameInfo WebPMuxFrameInfo;
typedef struct WebPMuxAnimParams WebPMuxAnimParams;

typedef enum WebPMuxError {
  WEBP_MUX_OK                 =  1,
  WEBP_MUX_NOT_FOUND          =  0,
  WEBP_MUX_INVALID_ARGUMENT   = -1,
  WEBP_MUX_BAD_DATA           = -2,
  WEBP_MUX_MEMORY_ERROR       = -3,
  WEBP_MUX_NOT_ENOUGH_DATA    = -4
} WebPMuxError;

typedef enum WebPChunkId {
  WEBP_CHUNK_VP8X,     
  WEBP_CHUNK_ICCP,     
  WEBP_CHUNK_ANIM,     
  WEBP_CHUNK_ANMF,     
  WEBP_CHUNK_FRGM,     
  WEBP_CHUNK_ALPHA,    
  WEBP_CHUNK_IMAGE,    
  WEBP_CHUNK_EXIF,     
  WEBP_CHUNK_XMP,      
  WEBP_CHUNK_UNKNOWN,  
  WEBP_CHUNK_NIL
} WebPChunkId;


WEBP_EXTERN(int) WebPGetMuxVersion(void);


WEBP_EXTERN(WebPMux*) WebPNewInternal(int);

static WEBP_INLINE WebPMux* WebPMuxNew(void) {
  return WebPNewInternal(WEBP_MUX_ABI_VERSION);
}

WEBP_EXTERN(void) WebPMuxDelete(WebPMux* mux);


WEBP_EXTERN(WebPMux*) WebPMuxCreateInternal(const WebPData*, int, int);

static WEBP_INLINE WebPMux* WebPMuxCreate(const WebPData* bitstream,
                                          int copy_data) {
  return WebPMuxCreateInternal(bitstream, copy_data, WEBP_MUX_ABI_VERSION);
}



WEBP_EXTERN(WebPMuxError) WebPMuxSetChunk(
    WebPMux* mux, const char fourcc[4], const WebPData* chunk_data,
    int copy_data);

WEBP_EXTERN(WebPMuxError) WebPMuxGetChunk(
    const WebPMux* mux, const char fourcc[4], WebPData* chunk_data);

WEBP_EXTERN(WebPMuxError) WebPMuxDeleteChunk(
    WebPMux* mux, const char fourcc[4]);


struct WebPMuxFrameInfo {
  WebPData    bitstream;  
                          
  int         x_offset;   
  int         y_offset;   
  int         duration;   

  WebPChunkId id;         
                          
  WebPMuxAnimDispose dispose_method;  
  WebPMuxAnimBlend   blend_method;    
  uint32_t    pad[1];     
};

WEBP_EXTERN(WebPMuxError) WebPMuxSetImage(
    WebPMux* mux, const WebPData* bitstream, int copy_data);

WEBP_EXTERN(WebPMuxError) WebPMuxPushFrame(
    WebPMux* mux, const WebPMuxFrameInfo* frame, int copy_data);

WEBP_EXTERN(WebPMuxError) WebPMuxGetFrame(
    const WebPMux* mux, uint32_t nth, WebPMuxFrameInfo* frame);

WEBP_EXTERN(WebPMuxError) WebPMuxDeleteFrame(WebPMux* mux, uint32_t nth);


struct WebPMuxAnimParams {
  uint32_t bgcolor;  
                     
                     
                     
                     
  int loop_count;    
};

WEBP_EXTERN(WebPMuxError) WebPMuxSetAnimationParams(
    WebPMux* mux, const WebPMuxAnimParams* params);

WEBP_EXTERN(WebPMuxError) WebPMuxGetAnimationParams(
    const WebPMux* mux, WebPMuxAnimParams* params);


WEBP_EXTERN(WebPMuxError) WebPMuxGetFeatures(const WebPMux* mux,
                                             uint32_t* flags);

WEBP_EXTERN(WebPMuxError) WebPMuxNumChunks(const WebPMux* mux,
                                           WebPChunkId id, int* num_elements);

// Note: The content of 'assembled_data' will be ignored and overwritten.
WEBP_EXTERN(WebPMuxError) WebPMuxAssemble(WebPMux* mux,
                                          WebPData* assembled_data);


#if defined(__cplusplus) || defined(c_plusplus)
}    
#endif

#endif  
