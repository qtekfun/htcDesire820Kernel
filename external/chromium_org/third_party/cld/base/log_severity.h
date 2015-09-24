// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_LOG_SEVERITY_H_
#define BASE_LOG_SEVERITY_H_

#include "base/port.h"
#include "base/commandlineflags.h"

typedef int LogSeverity;

const int INFO = 0, WARNING = 1, ERROR = 2, FATAL = 3, NUM_SEVERITIES = 4;

#ifdef NDEBUG
#define DFATAL_LEVEL ERROR
#else
#define DFATAL_LEVEL FATAL
#endif

extern const char* const LogSeverityNames[NUM_SEVERITIES];

DECLARE_int32(v);
DECLARE_bool(silent_init);

#ifdef NDEBUG
enum { DEBUG_MODE = 0 };
#else
enum { DEBUG_MODE = 1 };
#endif

#endif  
