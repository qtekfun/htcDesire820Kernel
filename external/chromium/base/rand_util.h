// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_RAND_UTIL_H_
#define BASE_RAND_UTIL_H_
#pragma once

#include "base/base_api.h"
#include "base/basictypes.h"

namespace base {

BASE_API uint64 RandUint64();

BASE_API int RandInt(int min, int max);

BASE_API uint64 RandGenerator(uint64 max);

BASE_API double RandDouble();

}  

#endif  
