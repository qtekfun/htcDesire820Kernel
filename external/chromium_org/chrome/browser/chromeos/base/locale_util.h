// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_CHROMEOS_BASE_LOCALE_UTIL_H_
#define CHROME_BROWSER_CHROMEOS_BASE_LOCALE_UTIL_H_

#include <string>

#include "base/memory/scoped_ptr.h"

namespace base {

template <typename T>
class Callback;

}  

namespace chromeos {
namespace locale_util {

typedef base::Callback<void(const std::string&, const std::string&, bool)>
    SwitchLanguageCallback;

void SwitchLanguage(const std::string& locale,
                    bool enableLocaleKeyboardLayouts,
                    scoped_ptr<SwitchLanguageCallback> callback);

}  
}  

#endif  
