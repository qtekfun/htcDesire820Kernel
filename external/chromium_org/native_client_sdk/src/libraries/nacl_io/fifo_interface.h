// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRARIES_NACL_IO_FIFO_INTERFACE_H_
#define LIBRARIES_NACL_IO_FIFO_INTERFACE_H_

#include <stdint.h>
#include <stdlib.h>

namespace nacl_io {

class FIFOInterface {
 public:
  virtual ~FIFOInterface() {};

  virtual bool IsEmpty() = 0;
  virtual bool IsFull() = 0;
  virtual bool Resize(size_t len) = 0;

  virtual size_t ReadAvailable() = 0;
  virtual size_t WriteAvailable() = 0;
};

}  

#endif  
