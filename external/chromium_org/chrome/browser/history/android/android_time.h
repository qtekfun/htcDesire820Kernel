// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_ANDROID_ANDROID_TIME_H_
#define CHROME_BROWSER_HISTORY_ANDROID_ANDROID_TIME_H_

#include "base/time/time.h"

namespace history {

inline base::Time FromDatabaseTime(int64 milliseconds) {
  return base::TimeDelta::FromMilliseconds(milliseconds) +
      base::Time::UnixEpoch();
}

inline int64 ToDatabaseTime(const base::Time& time) {
  return (time - base::Time::UnixEpoch()).InMilliseconds();
}

}  

#endif  
