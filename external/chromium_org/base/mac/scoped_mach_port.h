// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_SCOPED_MACH_PORT_H_
#define BASE_MAC_SCOPED_MACH_PORT_H_

#include <mach/mach.h>

#include "base/basictypes.h"
#include "base/base_export.h"

namespace base {
namespace mac {

class BASE_EXPORT ScopedMachPort {
 public:
  
  explicit ScopedMachPort(mach_port_t port);

  ~ScopedMachPort();

  void reset(mach_port_t port = MACH_PORT_NULL);

  operator mach_port_t() const {
    return port_;
  }

  mach_port_t get() const {
    return port_;
  }

 private:
  mach_port_t port_;

  DISALLOW_COPY_AND_ASSIGN(ScopedMachPort);
};

}  
}  

#endif  
