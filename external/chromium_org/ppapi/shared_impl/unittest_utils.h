// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_UNITTEST_UTILS_H_
#define PPAPI_SHARED_IMPL_UNITTEST_UTILS_H_

#include "ppapi/c/pp_var.h"

namespace ppapi {

bool TestEqual(const PP_Var& expected, const PP_Var& actual);

}  

#endif  
