// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SXS_LINUX_H_
#define CHROME_BROWSER_SXS_LINUX_H_

#include "base/compiler_specific.h"

namespace base {
class FilePath;
}

namespace sxs_linux {

void AddChannelMarkToUserDataDir();

bool ShouldMigrateUserDataDir() WARN_UNUSED_RESULT;

int MigrateUserDataDir() WARN_UNUSED_RESULT;

}  

#endif  
