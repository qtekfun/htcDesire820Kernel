// Copyright 2011 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#ifndef WEBP_DEC_WEBPI_H_
#define WEBP_DEC_WEBPI_H_

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include "../utils/rescaler.h"
#include "./decode_vp8.h"


typedef struct WebPDecParams WebPDecParams;
typedef int (*OutputFunc)(const VP8Io* const io, WebPDecParams* const p);
typedef int (*OutputRowFunc)(WebPDecParams* const p, int y_pos);

struct WebPDecParams {
  WebPDecBuffer* output;             
  uint8_t* tmp_y, *tmp_u, *tmp_v;    
                                     

  int last_y;                 
  const WebPDecoderOptions* options;  
  
  WebPRescaler scaler_y, scaler_u, scaler_v, scaler_a;
  void* memory;                  

  OutputFunc emit;               
  OutputFunc emit_alpha;         
  OutputRowFunc emit_alpha_row;  
};

void WebPResetDecParams(WebPDecParams* const params);


typedef struct {
  const uint8_t* data;         
  size_t data_size;            
  size_t offset;               
  const uint8_t* alpha_data;   
  size_t alpha_data_size;      
  size_t compressed_size;      
  size_t riff_size;            
  int is_lossless;             
} WebPHeaderStructure;

VP8StatusCode WebPParseHeaders(WebPHeaderStructure* const headers);


void WebPInitCustomIo(WebPDecParams* const params, VP8Io* const io);

int WebPIoInitFromOptions(const WebPDecoderOptions* const options,
                          VP8Io* const io, WEBP_CSP_MODE src_colorspace);


VP8StatusCode WebPAllocateDecBuffer(int width, int height,
                                    const WebPDecoderOptions* const options,
                                    WebPDecBuffer* const buffer);

void WebPCopyDecBuffer(const WebPDecBuffer* const src,
                       WebPDecBuffer* const dst);

void WebPGrabDecBuffer(WebPDecBuffer* const src, WebPDecBuffer* const dst);




#if defined(__cplusplus) || defined(c_plusplus)
}    
#endif

#endif  
