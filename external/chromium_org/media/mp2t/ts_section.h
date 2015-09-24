// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MP2T_TS_SECTION_H_
#define MEDIA_MP2T_TS_SECTION_H_

namespace media {
namespace mp2t {

class TsSection {
 public:
  
  enum SpecialPid {
    kPidPat = 0x0,
    kPidCat = 0x1,
    kPidTsdt = 0x2,
    kPidNullPacket = 0x1fff,
    kPidMax = 0x1fff,
  };

  virtual ~TsSection() {}

  
  
  virtual bool Parse(bool payload_unit_start_indicator,
                     const uint8* buf, int size) = 0;

  
  
  virtual void Flush() = 0;

  
  virtual void Reset() = 0;
};

}  
}  

#endif
