// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_CHROMEDRIVER_TEST_UTIL_H_
#define CHROME_TEST_CHROMEDRIVER_TEST_UTIL_H_

#include <string>

#include "base/basictypes.h"

#if defined(OS_WIN)
#include <windows.h>
#elif defined(OS_MACOSX)
#include <Carbon/Carbon.h>
#include "base/mac/scoped_cftyperef.h"
#endif

class RestoreKeyboardLayoutOnDestruct {
 public:
  RestoreKeyboardLayoutOnDestruct();
  ~RestoreKeyboardLayoutOnDestruct();

 private:
#if defined(OS_WIN)
  HKL layout_;
#elif defined(OS_MACOSX)
  base::ScopedCFTypeRef<TISInputSourceRef> layout_;
#endif

  DISALLOW_COPY_AND_ASSIGN(RestoreKeyboardLayoutOnDestruct);
};

#if defined(OS_WIN)
bool SwitchKeyboardLayout(const std::string& input_locale_identifier);
#endif  

#if defined(OS_MACOSX)
bool SwitchKeyboardLayout(const std::string& input_source_id);
#endif  

#endif  
