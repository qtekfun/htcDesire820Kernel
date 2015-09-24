// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_STRINGS_STRINGIZE_MACROS_H_
#define BASE_STRINGS_STRINGIZE_MACROS_H_

#include "build/build_config.h"

#define STRINGIZE_NO_EXPANSION(x) #x

#define STRINGIZE(x) STRINGIZE_NO_EXPANSION(x)

#endif  
