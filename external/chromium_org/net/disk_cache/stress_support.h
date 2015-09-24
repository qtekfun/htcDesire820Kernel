// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_STRESS_SUPPORT_H_
#define NET_DISK_CACHE_STRESS_SUPPORT_H_

#include "base/logging.h"

namespace disk_cache {




#if defined(NET_BUILD_STRESS_CACHE)
#define STRESS_NOTREACHED() NOTREACHED()
#define STRESS_DCHECK(a) DCHECK(a)
#else
#define STRESS_NOTREACHED() {}
#define STRESS_DCHECK(a) {}
#endif

}  

#endif  
