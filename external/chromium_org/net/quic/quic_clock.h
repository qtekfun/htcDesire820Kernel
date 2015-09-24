// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_CLOCK_H_
#define NET_QUIC_QUIC_CLOCK_H_

#include "base/basictypes.h"
#include "net/base/net_export.h"
#include "net/quic/quic_time.h"

namespace net {

typedef double WallTime;

class NET_EXPORT_PRIVATE QuicClock {
 public:
  QuicClock();
  virtual ~QuicClock();

  
  virtual QuicTime ApproximateNow() const;

  
  
  virtual QuicTime Now() const;

  
  
  virtual QuicWallTime WallNow() const;
};

}  

#endif  
