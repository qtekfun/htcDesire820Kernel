// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MP2T_TS_SECTION_PSI_H_
#define MEDIA_MP2T_TS_SECTION_PSI_H_

#include "base/compiler_specific.h"
#include "media/base/byte_queue.h"
#include "media/mp2t/ts_section.h"

namespace media {

class BitReader;

namespace mp2t {

class TsSectionPsi : public TsSection {
 public:
  TsSectionPsi();
  virtual ~TsSectionPsi();

  
  virtual bool Parse(bool payload_unit_start_indicator,
                     const uint8* buf, int size) OVERRIDE;
  virtual void Flush() OVERRIDE;
  virtual void Reset() OVERRIDE;

  
  virtual bool ParsePsiSection(BitReader* bit_reader) = 0;

  
  virtual void ResetPsiSection() = 0;

 private:
  void ResetPsiState();

  
  ByteQueue psi_byte_queue_;

  
  bool wait_for_pusi_;

  
  int leading_bytes_to_discard_;

  DISALLOW_COPY_AND_ASSIGN(TsSectionPsi);
};

}  
}  

#endif

