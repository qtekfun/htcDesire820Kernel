// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CDM_PPAPI_LIBVPX_CDM_VIDEO_DECODER_H_
#define MEDIA_CDM_PPAPI_LIBVPX_CDM_VIDEO_DECODER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "media/cdm/ppapi/api/content_decryption_module.h"
#include "media/cdm/ppapi/cdm_video_decoder.h"

struct vpx_codec_ctx;
struct vpx_image;

namespace media {

class LibvpxCdmVideoDecoder : public CdmVideoDecoder {
 public:
  explicit LibvpxCdmVideoDecoder(CdmHost* host);
  virtual ~LibvpxCdmVideoDecoder();

  
  virtual bool Initialize(const cdm::VideoDecoderConfig& config) OVERRIDE;
  virtual void Deinitialize() OVERRIDE;
  virtual void Reset() OVERRIDE;
  virtual cdm::Status DecodeFrame(const uint8_t* compressed_frame,
                                  int32_t compressed_frame_size,
                                  int64_t timestamp,
                                  cdm::VideoFrame* decoded_frame) OVERRIDE;
  virtual bool is_initialized() const OVERRIDE { return is_initialized_; }

  
  
  static bool IsValidOutputConfig(cdm::VideoFormat format,
                                  const cdm::Size& data_size);

 private:
  
  
  bool CopyVpxImageTo(cdm::VideoFrame* cdm_video_frame);

  bool is_initialized_;

  CdmHost* const host_;

  vpx_codec_ctx* vpx_codec_;
  vpx_image* vpx_image_;

  DISALLOW_COPY_AND_ASSIGN(LibvpxCdmVideoDecoder);
};

}  

#endif  
