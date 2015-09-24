// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRARIES_NACL_IO_FIFO_CHAR_H_
#define LIBRARIES_NACL_IO_FIFO_CHAR_H_

#include <vector>

#include "nacl_io/fifo_interface.h"

#include "sdk_util/macros.h"

namespace nacl_io {

class FIFOChar : public FIFOInterface {
 public:
  explicit FIFOChar(size_t size);
  virtual ~FIFOChar();

  virtual bool IsEmpty();
  virtual bool IsFull();
  virtual bool Resize(size_t len);

  size_t ReadAvailable();
  size_t WriteAvailable();

  
  
  size_t Peek(void* buf, size_t len);

  
  
  size_t Read(void* buf, size_t len);

  
  // written.
  size_t Write(const void* buf, size_t len);

private:
  char* buffer_;
  size_t size_;   
  size_t avail_;  
  size_t tail_;   

  DISALLOW_COPY_AND_ASSIGN(FIFOChar);
};

}  

#endif  
