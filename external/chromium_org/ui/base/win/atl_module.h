// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_WIN_ATL_MODULE_H_
#define UI_BASE_WIN_ATL_MODULE_H_

#include <atlbase.h>
#include <atlcom.h>

#include "base/basictypes.h"

namespace ui {
namespace win {

static void CreateATLModuleIfNeeded() {
  if (_pAtlModule == NULL) {
    
    CR_DEFINE_STATIC_LOCAL(CComModule, module, ());
  }
}

}  
}  

#endif  
