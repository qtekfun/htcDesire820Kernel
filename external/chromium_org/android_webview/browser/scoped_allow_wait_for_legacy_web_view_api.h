// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_BROWSER_SCOPED_ALLOW_WAIT_FOR_LEGACY_WEB_VIEW_API_H
#define ANDROID_WEBVIEW_BROWSER_SCOPED_ALLOW_WAIT_FOR_LEGACY_WEB_VIEW_API_H

#include "base/threading/thread_restrictions.h"

class ScopedAllowWaitForLegacyWebViewApi {
 private:
  base::ThreadRestrictions::ScopedAllowWait wait;
};

#endif  
