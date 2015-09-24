// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFERENCES_MAC_H_
#define CHROME_BROWSER_PREFERENCES_MAC_H_
#pragma once

#include <CoreFoundation/CoreFoundation.h>

#include "base/basictypes.h"


class MacPreferences {
 public:
  MacPreferences() {}
  virtual ~MacPreferences() {}

  virtual Boolean AppSynchronize(CFStringRef applicationID);

  virtual CFPropertyListRef CopyAppValue(CFStringRef key,
                                         CFStringRef applicationID);

  virtual Boolean AppValueIsForced(CFStringRef key, CFStringRef applicationID);

 private:
  DISALLOW_COPY_AND_ASSIGN(MacPreferences);
};

#endif  
