// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_CONTENT_PATHS_H_
#define CONTENT_PUBLIC_COMMON_CONTENT_PATHS_H_

#include "content/common/content_export.h"


namespace content {

enum {
  PATH_START = 4000,

  
  CHILD_PROCESS_EXE = PATH_START,

  
  DIR_TEST_DATA,

  
  DIR_MEDIA_LIBS,

  PATH_END
};

CONTENT_EXPORT void RegisterPathProvider();

}  

#endif  
