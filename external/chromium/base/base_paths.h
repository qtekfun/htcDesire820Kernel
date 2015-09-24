// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_BASE_PATHS_H_
#define BASE_BASE_PATHS_H_
#pragma once


#include "build/build_config.h"

#if defined(OS_WIN)
#include "base/base_paths_win.h"
#elif defined(OS_MACOSX)
#include "base/base_paths_mac.h"
#endif

namespace base {

enum {
  PATH_START = 0,

  DIR_CURRENT,  
  DIR_EXE,      
  DIR_MODULE,   
  DIR_TEMP,     
  FILE_EXE,     
  FILE_MODULE,  
                
                
  DIR_SOURCE_ROOT,  
                    
                    
#if defined(OS_POSIX)
  DIR_CACHE,    
                
                
                
                
#endif

  PATH_END
};

}  

#endif  
