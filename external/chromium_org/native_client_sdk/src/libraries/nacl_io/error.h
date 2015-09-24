// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef LIBRARIES_NACL_IO_ERROR_H_
#define LIBRARIES_NACL_IO_ERROR_H_

namespace nacl_io {

struct Error {
  
  
  Error(int error) : error(error) {}
  operator int() const { return error; }

  int error;
};

}  

#endif  
