// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef TOOLS_ANDROID_FORWARDER2_COMMON_H_
#define TOOLS_ANDROID_FORWARDER2_COMMON_H_

#include <stdarg.h>
#include <stdio.h>
#include <errno.h>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/logging.h"
#include "base/posix/eintr_wrapper.h"

#define PRESERVE_ERRNO_HANDLE_EINTR(Func)                     \
  do {                                                        \
    int local_errno = errno;                                  \
    (void) HANDLE_EINTR(Func);                                \
    errno = local_errno;                                      \
  } while (false);

#define SIGNAL_SAFE_LOG(Level, Msg) \
  RAW_LOG(Level, Msg);

namespace forwarder2 {


void PError(const char* msg);

void CloseFD(int fd);

template <int BufferSize>
class FixedSizeStringBuilder {
 public:
  FixedSizeStringBuilder() {
    Reset();
  }

  const char* buffer() const { return buffer_; }

  void Reset() {
    buffer_[0] = 0;
    write_ptr_ = buffer_;
  }

  
  
  int Append(const char* format, ...) PRINTF_FORMAT( 2, 3) {
    if (write_ptr_ >= buffer_ + BufferSize)
      return -1;
    va_list ap;
    va_start(ap, format);
    const int bytes_written = vsnprintf(
        write_ptr_, BufferSize - (write_ptr_ - buffer_), format, ap);
    va_end(ap);
    if (bytes_written > 0)
      write_ptr_ += bytes_written;
    return bytes_written;
  }

 private:
  char* write_ptr_;
  char buffer_[BufferSize];

  COMPILE_ASSERT(BufferSize >= 1, Size_of_buffer_must_be_at_least_one);
  DISALLOW_COPY_AND_ASSIGN(FixedSizeStringBuilder);
};

}  

#endif  
