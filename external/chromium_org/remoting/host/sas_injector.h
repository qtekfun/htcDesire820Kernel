// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_SAS_INJECTOR_H_
#define REMOTING_HOST_SAS_INJECTOR_H_

#include "base/memory/scoped_ptr.h"

namespace remoting {

class SasInjector {
 public:
  virtual ~SasInjector() {}

  
  virtual bool InjectSas() = 0;

  
  
  static scoped_ptr<SasInjector> Create();
};

}  

#endif REMOTING_HOST_SAS_INJECTOR_H_
