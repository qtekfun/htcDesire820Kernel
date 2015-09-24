// Copyright 2012 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license


#ifndef WEBP_WEBP_DEMUX_H_
#define WEBP_WEBP_DEMUX_H_

#include "./mux_types.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#define WEBP_DEMUX_ABI_VERSION 0x0101    

typedef struct WebPDemuxer WebPDemuxer;
typedef struct WebPIterator WebPIterator;
typedef struct WebPChunkIterator WebPChunkIterator;


WEBP_EXTERN(int) WebPGetDemuxVersion(void);


typedef enum WebPDemuxState {
  WEBP_DEMUX_PARSING_HEADER,  
  WEBP_DEMUX_PARSED_HEADER,   
  WEBP_DEMUX_DONE             
} WebPDemuxState;

WEBP_EXTERN(WebPDemuxer*) WebPDemuxInternal(
    const WebPData*, int, WebPDemuxState*, int);

static WEBP_INLINE WebPDemuxer* WebPDemux(const WebPData* data) {
  return WebPDemuxInternal(data, 0, NULL, WEBP_DEMUX_ABI_VERSION);
}

static WEBP_INLINE WebPDemuxer* WebPDemuxPartial(
    const WebPData* data, WebPDemuxState* state) {
  return WebPDemuxInternal(data, 1, state, WEBP_DEMUX_ABI_VERSION);
}

WEBP_EXTERN(void) WebPDemuxDelete(WebPDemuxer* dmux);


typedef enum WebPFormatFeature {
  WEBP_FF_FORMAT_FLAGS,  
  WEBP_FF_CANVAS_WIDTH,
  WEBP_FF_CANVAS_HEIGHT,
  WEBP_FF_LOOP_COUNT,
  WEBP_FF_BACKGROUND_COLOR,
  WEBP_FF_FRAME_COUNT    
                         
                         
                         
} WebPFormatFeature;

WEBP_EXTERN(uint32_t) WebPDemuxGetI(
    const WebPDemuxer* dmux, WebPFormatFeature feature);


struct WebPIterator {
  int frame_num;
  int num_frames;          
  int fragment_num;
  int num_fragments;
  int x_offset, y_offset;  
  int width, height;       
  int duration;            
  WebPMuxAnimDispose dispose_method;  
  int complete;   
                  
  WebPData fragment;  
                      
  int has_alpha;      
  WebPMuxAnimBlend blend_method;  

  uint32_t pad[2];         
  void* private_;          
};

WEBP_EXTERN(int) WebPDemuxGetFrame(
    const WebPDemuxer* dmux, int frame_number, WebPIterator* iter);

WEBP_EXTERN(int) WebPDemuxNextFrame(WebPIterator* iter);
WEBP_EXTERN(int) WebPDemuxPrevFrame(WebPIterator* iter);

WEBP_EXTERN(int) WebPDemuxSelectFragment(WebPIterator* iter, int fragment_num);

WEBP_EXTERN(void) WebPDemuxReleaseIterator(WebPIterator* iter);


struct WebPChunkIterator {
  
  
  int chunk_num;
  int num_chunks;
  WebPData chunk;    

  uint32_t pad[6];   
  void* private_;
};

WEBP_EXTERN(int) WebPDemuxGetChunk(const WebPDemuxer* dmux,
                                   const char fourcc[4], int chunk_number,
                                   WebPChunkIterator* iter);

WEBP_EXTERN(int) WebPDemuxNextChunk(WebPChunkIterator* iter);
WEBP_EXTERN(int) WebPDemuxPrevChunk(WebPChunkIterator* iter);

WEBP_EXTERN(void) WebPDemuxReleaseChunkIterator(WebPChunkIterator* iter);


#if defined(__cplusplus) || defined(c_plusplus)
}    
#endif

#endif  
