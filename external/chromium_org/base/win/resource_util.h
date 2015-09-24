// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_WIN_RESOURCE_UTIL_H__
#define BASE_WIN_RESOURCE_UTIL_H__

#include <windows.h>

#include "base/base_export.h"
#include "base/basictypes.h"

namespace base {
namespace win {

bool BASE_EXPORT GetResourceFromModule(HMODULE module,
                                       int resource_id,
                                       LPCTSTR resource_type,
                                       void** data,
                                       size_t* length);

bool BASE_EXPORT GetDataResourceFromModule(HMODULE module,
                                           int resource_id,
                                           void** data,
                                           size_t* length);

}  
}  

#endif  
