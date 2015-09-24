// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_KEYBOARD_DRIVEN_EVENT_REWRITER_H_
#define CHROME_BROWSER_CHROMEOS_KEYBOARD_DRIVEN_EVENT_REWRITER_H_

#include "base/basictypes.h"

namespace ui {
class KeyEvent;
}

namespace chromeos {

class KeyboardDrivenEventRewriter {
 public:
  KeyboardDrivenEventRewriter();
  ~KeyboardDrivenEventRewriter();

  
  
  
  bool RewriteIfKeyboardDrivenOnLoginScreen(ui::KeyEvent* event);

  
  bool RewriteForTesting(ui::KeyEvent* event);

 private:
  bool RewriteEvent(ui::KeyEvent* event);

  DISALLOW_COPY_AND_ASSIGN(KeyboardDrivenEventRewriter);
};

}  

#endif  
