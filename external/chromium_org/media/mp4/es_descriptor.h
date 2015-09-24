// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MP4_ES_DESCRIPTOR_H_
#define MEDIA_MP4_ES_DESCRIPTOR_H_

#include <vector>

#include "base/basictypes.h"
#include "media/base/media_export.h"

namespace media {

class BitReader;

namespace mp4 {

enum ObjectType {
  kForbidden = 0,
  kISO_14496_3 = 0x40,  
  kISO_13818_7_AAC_LC = 0x67,  
  kEAC3 = 0xa6  
};

class MEDIA_EXPORT ESDescriptor {
 public:
  
  static bool IsAAC(uint8 object_type);

  ESDescriptor();
  ~ESDescriptor();

  bool Parse(const std::vector<uint8>& data);

  uint8 object_type() const;
  const std::vector<uint8>& decoder_specific_info() const;

 private:
  enum Tag {
    kESDescrTag = 0x03,
    kDecoderConfigDescrTag = 0x04,
    kDecoderSpecificInfoTag = 0x05
  };

  bool ParseDecoderConfigDescriptor(BitReader* reader);
  bool ParseDecoderSpecificInfo(BitReader* reader);

  uint8 object_type_;
  std::vector<uint8> decoder_specific_info_;
};

}  

}  

#endif  
