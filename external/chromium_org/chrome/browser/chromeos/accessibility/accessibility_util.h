// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_ACCESSIBILITY_ACCESSIBILITY_UTIL_H_
#define CHROME_BROWSER_CHROMEOS_ACCESSIBILITY_ACCESSIBILITY_UTIL_H_

class Browser;

namespace chromeos {
namespace accessibility {

void EnableVirtualKeyboard(bool enabled);

bool IsVirtualKeyboardEnabled();

void ShowAccessibilityHelp(Browser* browser);

}  
}  

#endif  
