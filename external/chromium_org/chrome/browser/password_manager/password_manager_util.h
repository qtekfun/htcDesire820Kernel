// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_MANAGER_UTIL_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_MANAGER_UTIL_H_

#include "base/basictypes.h"
#include "ui/gfx/native_widget_types.h"

namespace password_manager_util {

enum OsPasswordStatus {
  PASSWORD_STATUS_UNKNOWN = 0,
  PASSWORD_STATUS_UNSUPPORTED,
  PASSWORD_STATUS_BLANK,
  PASSWORD_STATUS_NONBLANK,
  PASSWORD_STATUS_WIN_DOMAIN,
  
  
  
  MAX_PASSWORD_STATUS
};

bool AuthenticateUser(gfx::NativeWindow window);

OsPasswordStatus GetOsPasswordStatus();

}  

#endif  
