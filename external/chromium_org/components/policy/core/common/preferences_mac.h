// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_PREFERENCES_MAC_H_
#define COMPONENTS_POLICY_CORE_COMMON_PREFERENCES_MAC_H_

#include <CoreFoundation/CoreFoundation.h>

#include "base/basictypes.h"
#include "components/policy/policy_export.h"


class POLICY_EXPORT MacPreferences {
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
