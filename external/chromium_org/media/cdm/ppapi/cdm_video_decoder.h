// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CDM_PPAPI_CDM_VIDEO_DECODER_H_
#define MEDIA_CDM_PPAPI_CDM_VIDEO_DECODER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "media/cdm/ppapi/api/content_decryption_module.h"
#include "media/cdm/ppapi/clear_key_cdm_common.h"

namespace media {

class CdmVideoDecoder {
 public:
  virtual ~CdmVideoDecoder() {}
  virtual bool Initialize(const cdm::VideoDecoderConfig& config) = 0;
  virtual void Deinitialize() = 0;
  virtual void Reset() = 0;
  virtual bool is_initialized() const = 0;

  
  
  
  
  
  
  
  virtual cdm::Status DecodeFrame(const uint8_t* compressed_frame,
                                  int32_t compressed_frame_size,
                                  int64_t timestamp,
                                  cdm::VideoFrame* decoded_frame) = 0;
};

scoped_ptr<CdmVideoDecoder> CreateVideoDecoder(
    ClearKeyCdmHost* host, const cdm::VideoDecoderConfig& config);

}  

#endif  
