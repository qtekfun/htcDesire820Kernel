// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_NACL_COMMON_NACL_PATHS_H_
#define COMPONENTS_NACL_COMMON_NACL_PATHS_H_

#include "build/build_config.h"


namespace nacl {

enum {
  PATH_START = 9000,

#if defined(OS_LINUX)
  FILE_NACL_HELPER = PATH_START, 
  FILE_NACL_HELPER_BOOTSTRAP,    
#endif

  PATH_END
};

void RegisterPathProvider();

}  

#endif  
