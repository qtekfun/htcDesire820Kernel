// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_RESOURCE_UTIL_H__
#define BASE_RESOURCE_UTIL_H__
#pragma once

#include <windows.h>

#include "base/base_api.h"
#include "base/basictypes.h"

namespace base {

bool BASE_API GetDataResourceFromModule(HMODULE module, int resource_id,
                                        void** data, size_t* length);
}  

#endif  
