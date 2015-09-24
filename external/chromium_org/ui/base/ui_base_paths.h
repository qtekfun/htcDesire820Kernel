// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_UI_BASE_PATHS_H_
#define UI_BASE_UI_BASE_PATHS_H_

#include "build/build_config.h"
#include "ui/base/ui_export.h"


namespace ui {

enum {
  PATH_START = 3000,

  DIR_LOCALES,              

  
  DIR_TEST_DATA,            

#if defined(OS_ANDROID)
  DIR_RESOURCE_PAKS_ANDROID,
#endif

  PATH_END
};

UI_EXPORT void RegisterPathProvider();

}  

#endif  
