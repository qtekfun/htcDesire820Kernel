// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_APP_MODE_APP_MODE_UTILS_H_
#define CHROME_BROWSER_APP_MODE_APP_MODE_UTILS_H_

namespace chrome {

bool IsCommandAllowedInAppMode(int command_id);

bool IsRunningInAppMode();

bool IsRunningInForcedAppMode();

}  

#endif  
