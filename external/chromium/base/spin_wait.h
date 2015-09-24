// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.




#ifndef BASE_SPIN_WAIT_H_
#define BASE_SPIN_WAIT_H_
#pragma once

#include "base/threading/platform_thread.h"
#include "base/time.h"


#define SPIN_FOR_1_SECOND_OR_UNTIL_TRUE(expression) \
    SPIN_FOR_TIMEDELTA_OR_UNTIL_TRUE(base::TimeDelta::FromSeconds(1), \
                                     (expression))

#define SPIN_FOR_TIMEDELTA_OR_UNTIL_TRUE(delta, expression) do { \
  base::TimeTicks start = base::TimeTicks::Now(); \
  const base::TimeDelta kTimeout = delta; \
    while (!(expression)) { \
      if (kTimeout < base::TimeTicks::Now() - start) { \
      EXPECT_LE((base::TimeTicks::Now() - start).InMilliseconds(), \
                kTimeout.InMilliseconds()) << "Timed out"; \
        break; \
      } \
      base::PlatformThread::Sleep(50); \
    } \
  } while (0)

#endif  
