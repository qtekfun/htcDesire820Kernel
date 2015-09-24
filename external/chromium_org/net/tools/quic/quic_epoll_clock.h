// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_QUIC_QUIC_EPOLL_CLOCK_H_
#define NET_TOOLS_QUIC_QUIC_EPOLL_CLOCK_H_

#include "base/compiler_specific.h"
#include "net/quic/quic_clock.h"
#include "net/quic/quic_time.h"

namespace net {

class EpollServer;

namespace tools {

class QuicEpollClock : public QuicClock {
 public:
  explicit QuicEpollClock(EpollServer* epoll_server);
  virtual ~QuicEpollClock();

  
  virtual QuicTime ApproximateNow() const OVERRIDE;

  
  
  virtual QuicTime Now() const OVERRIDE;

 protected:
  EpollServer* epoll_server_;
};

}  
}  

#endif  
