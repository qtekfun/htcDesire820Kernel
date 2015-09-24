// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_STRINGIZE_MACROS_H_
#define BASE_STRINGIZE_MACROS_H_
#pragma once

#include "build/build_config.h"

#define STRINGIZE_NO_EXPANSION(x) #x

#define STRINGIZE(x) STRINGIZE_NO_EXPANSION(x)

#if defined(OS_WIN)

#define LSTRINGIZE_NO_EXPANSION(x) L ## #x
#define TO_L_STRING_NO_EXPANSION(x) L ## x

#define LSTRINGIZE(x) LSTRINGIZE_NO_EXPANSION(x)

#define TO_L_STRING(x) TO_L_STRING_NO_EXPANSION(x)

#endif  

#endif  
