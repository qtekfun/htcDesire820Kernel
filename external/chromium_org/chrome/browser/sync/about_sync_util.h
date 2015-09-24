// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ABOUT_SYNC_UTIL_H_
#define CHROME_BROWSER_SYNC_ABOUT_SYNC_UTIL_H_

#include "base/memory/scoped_ptr.h"

class ProfileSyncService;

namespace base {
class DictionaryValue;
}

extern const char kIdentityTitle[];
extern const char kDetailsKey[];

namespace sync_ui_util {
scoped_ptr<base::DictionaryValue> ConstructAboutInformation(
    ProfileSyncService* service);
}

#endif  
