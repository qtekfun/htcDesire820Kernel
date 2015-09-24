// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_HISTOGRAM_FETCHER_H_
#define CONTENT_PUBLIC_BROWSER_HISTOGRAM_FETCHER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/time/time.h"
#include "content/common/content_export.h"

namespace base {
class MessageLoop;
}

namespace content {

CONTENT_EXPORT void FetchHistogramsAsynchronously(
    base::MessageLoop* callback_thread,
    const base::Closure& callback,
    base::TimeDelta wait_time);

}  

#endif  
