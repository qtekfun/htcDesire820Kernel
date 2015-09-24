// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFERENCES_MOCK_MAC_H_
#define CHROME_BROWSER_PREFERENCES_MOCK_MAC_H_
#pragma once

#include "base/mac/scoped_cftyperef.h"
#include "chrome/browser/preferences_mac.h"

class MockPreferences : public MacPreferences {
 public:
  MockPreferences();
  virtual ~MockPreferences();

  virtual Boolean AppSynchronize(CFStringRef applicationID);

  virtual CFPropertyListRef CopyAppValue(CFStringRef key,
                                         CFStringRef applicationID);

  virtual Boolean AppValueIsForced(CFStringRef key, CFStringRef applicationID);

  
  void AddTestItem(CFStringRef key, CFPropertyListRef value, bool is_forced);

 private:
  base::mac::ScopedCFTypeRef<CFMutableDictionaryRef> values_;
  base::mac::ScopedCFTypeRef<CFMutableSetRef> forced_;
};

#endif  
