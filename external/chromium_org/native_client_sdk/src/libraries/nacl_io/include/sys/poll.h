/* Copyright (c) 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file. */

#ifndef LIBRARIES_NACL_IO_INCLUDE_SYS_POLL_H_
#define LIBRARIES_NACL_IO_INCLUDE_SYS_POLL_H_

#include <stdint.h>

#include "sdk_util/macros.h"

EXTERN_C_BEGIN


#define POLLIN   0x0001   
#define POLLPRI  0x0002   
#define POLLOUT  0x0004   
#define POLLERR  0x0008   
#define POLLHUP  0x0010   
#define POLLNVAL 0x0020   

typedef int nfds_t;

struct pollfd {
  int fd;
  uint16_t events;
  uint16_t revents;
};

int poll(struct pollfd *__fds, nfds_t __nfds, int __timeout);

EXTERN_C_END

#endif  
