// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_APP_LIST_UTIL_H_
#define CHROME_BROWSER_UI_APP_LIST_APP_LIST_UTIL_H_

#include "base/callback_forward.h"

class PrefService;

void SetupShowAppLauncherPromoFieldTrial(PrefService* local_state);

bool IsAppLauncherEnabled();

bool ShouldShowAppLauncherPromo();

#endif  
