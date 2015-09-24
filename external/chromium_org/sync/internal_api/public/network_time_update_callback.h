// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_NETWORK_TIME_UPDATE_CALLBACK_H_
#define SYNC_INTERNAL_API_PUBLIC_NETWORK_TIME_UPDATE_CALLBACK_H_

#include "base/callback.h"
#include "base/time/time.h"

namespace syncer {

typedef base::Callback<void(const base::Time&,
                            const base::TimeDelta&,
                            const base::TimeDelta&)> NetworkTimeUpdateCallback;

}  

#endif  
