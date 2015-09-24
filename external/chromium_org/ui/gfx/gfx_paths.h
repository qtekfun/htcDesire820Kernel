// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_GFX_PATHS_H_
#define UI_GFX_GFX_PATHS_H_

#include "ui/gfx/gfx_export.h"


namespace gfx {

enum {
  PATH_START = 2000,

  
  DIR_TEST_DATA,            

  PATH_END
};

GFX_EXPORT void RegisterPathProvider();

}  

#endif  
