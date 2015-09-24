// Copyright (c) 2008, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef BASE_RAW_PRINTER_H_
#define BASE_RAW_PRINTER_H_

#include <config.h>
#include "base/basictypes.h"

namespace base {

class RawPrinter {
 public:
  
  
  
  RawPrinter(char* buf, int length);

  
  
  int length() const { return (ptr_ - base_); }

  
  int space_left() const { return (limit_ - ptr_); }

  
  
  
  void Printf(const char* format, ...)
#ifdef HAVE___ATTRIBUTE__
  __attribute__ ((__format__ (__printf__, 2, 3)))
#endif
;

 private:
  
  
  
  
  
  
  char* base_;          
  char* ptr_;           
  char* limit_;         

  DISALLOW_COPY_AND_ASSIGN(RawPrinter);
};

}

#endif  
