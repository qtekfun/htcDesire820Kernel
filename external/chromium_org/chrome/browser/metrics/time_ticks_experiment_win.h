// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_METRICS_TIME_TICKS_EXPERIMENT_WIN_H_
#define CHROME_BROWSER_METRICS_TIME_TICKS_EXPERIMENT_WIN_H_

#include "build/build_config.h"

#if defined(OS_WIN)

namespace chrome {

void CollectTimeTicksStats();

}  

#endif  

#endif  
