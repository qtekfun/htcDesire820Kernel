// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_DEBUG_LEAK_ANNOTATIONS_H_
#define BASE_DEBUG_LEAK_ANNOTATIONS_H_
#pragma once

#include "build/build_config.h"

#if defined(OS_LINUX) && defined(USE_HEAPCHECKER)

#include "third_party/tcmalloc/chromium/src/google/heap-checker.h"

#define ANNOTATE_SCOPED_MEMORY_LEAK \
    HeapLeakChecker::Disabler heap_leak_checker_disabler

#else

#define ANNOTATE_SCOPED_MEMORY_LEAK

#endif

#endif  
