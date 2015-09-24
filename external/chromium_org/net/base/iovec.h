// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_IOVEC_H_
#define NET_BASE_IOVEC_H_

#if defined(OS_POSIX)
#include <sys/uio.h>
#else
struct iovec {
  void* iov_base;  
  size_t iov_len;  
};
#endif  

#endif  
