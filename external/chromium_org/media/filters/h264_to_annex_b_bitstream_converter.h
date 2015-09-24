// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FILTERS_H264_TO_ANNEX_B_BITSTREAM_CONVERTER_H_
#define MEDIA_FILTERS_H264_TO_ANNEX_B_BITSTREAM_CONVERTER_H_

#include "base/basictypes.h"
#include "media/base/media_export.h"

namespace media {

class MEDIA_EXPORT H264ToAnnexBBitstreamConverter {
 public:
  H264ToAnnexBBitstreamConverter();
  ~H264ToAnnexBBitstreamConverter();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  uint32 ParseConfigurationAndCalculateSize(const uint8* configuration_record,
                                            uint32 configuration_record_size);

  
  
  
  
  
  
  
  
  
  
  
  
  
  uint32 CalculateNeededOutputBufferSize(const uint8* input,
                                         uint32 input_size) const;

  
  
  
  
  
  
  // In case of failed conversion object H264BitstreamConverter may have written
  
  
  
  
  
  
  
  
  
  //     Pointer to buffer where the output should be written to.
  
  
  //     bytes written to output after successful call.
  
  
  
  
  
  bool ConvertAVCDecoderConfigToByteStream(const uint8* input,
                                           uint32 input_size,
                                           uint8* output,
                                           uint32* output_size);

  
  
  
  
  
  // In case of failed conversion object H264BitstreamConverter may have written
  
  
  
  
  
  
  
  
  
  //     Pointer to buffer where the output should be written to.
  
  
  //     bytes written to output after successful call.
  
  
  
  
  
  bool ConvertNalUnitStreamToByteStream(const uint8* input, uint32 input_size,
                                        uint8* output, uint32* output_size);

 private:
  
  bool configuration_processed_;
  
  bool first_nal_unit_in_access_unit_;
  
  uint8 nal_unit_length_field_width_;

  DISALLOW_COPY_AND_ASSIGN(H264ToAnnexBBitstreamConverter);
};

}  

#endif  

