// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IOS_PUBLIC_CONSUMER_BASE_UTIL_H_
#define IOS_PUBLIC_CONSUMER_BASE_UTIL_H_

namespace ios {

bool IsRunningOnIOS6OrLater();

bool IsRunningOnOrLater(int major, int minor, int bug_fix);

}  

#endif  
