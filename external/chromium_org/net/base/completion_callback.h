// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_COMPLETION_CALLBACK_H__
#define NET_BASE_COMPLETION_CALLBACK_H__

#include "base/callback.h"
#include "base/cancelable_callback.h"

namespace net {

typedef base::Callback<void(int)> CompletionCallback;

typedef base::Callback<void(int64)> Int64CompletionCallback;

typedef base::CancelableCallback<void(int)> CancelableCompletionCallback;

}  

#endif  
