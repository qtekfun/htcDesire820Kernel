// Copyright 2012 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#ifndef WEBP_ENC_VP8LI_H_
#define WEBP_ENC_VP8LI_H_

#include "./histogram.h"
#include "../utils/bit_writer.h"
#include "../webp/encode.h"
#include "../webp/format_constants.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef struct {
  const WebPConfig* config_;    
  const WebPPicture* pic_;      

  uint32_t* argb_;              
  uint32_t* argb_scratch_;      
                                
  uint32_t* transform_data_;    
  int       current_width_;     

  
  int histo_bits_;
  int transform_bits_;
  int cache_bits_;        

  
  int use_cross_color_;
  int use_subtract_green_;
  int use_predict_;
  int use_palette_;
  int palette_size_;
  uint32_t palette_[MAX_PALETTE_SIZE];
} VP8LEncoder;


int VP8LEncodeImage(const WebPConfig* const config,
                    const WebPPicture* const picture);

WebPEncodingError VP8LEncodeStream(const WebPConfig* const config,
                                   const WebPPicture* const picture,
                                   VP8LBitWriter* const bw);


#if defined(__cplusplus) || defined(c_plusplus)
}    
#endif

#endif  
