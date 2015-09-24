// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MP2T_TS_PACKET_H_
#define MEDIA_MP2T_TS_PACKET_H_

#include "base/basictypes.h"

namespace media {

class BitReader;

namespace mp2t {

class TsPacket {
 public:
  static const int kPacketSize = 188;

  
  
  static int Sync(const uint8* buf, int size);

  
  
  
  static TsPacket* Parse(const uint8* buf, int size);

  ~TsPacket();

  
  bool payload_unit_start_indicator() const {
    return payload_unit_start_indicator_;
  }
  int pid() const { return pid_; }
  int continuity_counter() const { return continuity_counter_; }
  bool discontinuity_indicator() const { return discontinuity_indicator_; }
  bool random_access_indicator() const { return random_access_indicator_; }

  
  const uint8* payload() const { return payload_; }
  int payload_size() const { return payload_size_; }

 private:
  TsPacket();

  
  
  bool ParseHeader(const uint8* buf);
  bool ParseAdaptationField(BitReader* bit_reader,
                            int adaptation_field_length);

  
  const uint8* payload_;
  int payload_size_;

  
  bool payload_unit_start_indicator_;
  int pid_;
  int continuity_counter_;

  
  bool discontinuity_indicator_;
  bool random_access_indicator_;

  DISALLOW_COPY_AND_ASSIGN(TsPacket);
};

}  
}  

#endif

