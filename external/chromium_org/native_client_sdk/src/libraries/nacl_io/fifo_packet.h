// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRARIES_NACL_IO_FIFO_PACKET_H_
#define LIBRARIES_NACL_IO_FIFO_PACKET_H_

#include <string.h>

#include <list>
#include <vector>

#include "nacl_io/fifo_interface.h"
#include "ppapi/c/pp_resource.h"

#include "sdk_util/macros.h"

namespace nacl_io {

class Packet;

class  FIFOPacket : public FIFOInterface {
 public:
  explicit FIFOPacket(size_t size);
  virtual ~FIFOPacket();

  virtual bool IsEmpty();
  virtual bool IsFull();
  virtual bool Resize(size_t len);

  size_t ReadAvailable();
  size_t WriteAvailable();

  
  Packet* PeekPacket();

  
  Packet* ReadPacket();

  
  void WritePacket(Packet* packet);

 private:
  std::list<Packet*> packets_;
  uint32_t max_bytes_;
  uint32_t cur_bytes_;

  DISALLOW_COPY_AND_ASSIGN(FIFOPacket);
};

}  

#endif  
