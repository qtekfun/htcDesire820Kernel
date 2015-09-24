// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_GPU_X_UTIL_H_
#define CHROME_GPU_X_UTIL_H_


#include "build/build_config.h"
#include "content/common/gpu/gpu_config.h"

#if defined(USE_X11)

namespace content {


typedef unsigned long XID;

extern "C" {

typedef struct _XDisplay Display;
typedef struct __GLXcontextRec *GLXContext;

}  


class ScopedPtrXFree {
 public:
  void operator()(void* x) const;
};

}  

#endif  

#endif  
