// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CONGESTION_CONTROL_CUBE_ROOT_H_
#define NET_QUIC_CONGESTION_CONTROL_CUBE_ROOT_H_

#include "base/basictypes.h"
#include "net/base/net_export.h"

namespace net {

class NET_EXPORT_PRIVATE CubeRoot {
 public:
  
  
  static uint32 Root(uint64 a);
};

}  
#endif  
