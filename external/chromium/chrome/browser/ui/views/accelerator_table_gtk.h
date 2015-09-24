// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_ACCELERATOR_TABLE_GTK_H_
#define CHROME_BROWSER_UI_VIEWS_ACCELERATOR_TABLE_GTK_H_
#pragma once

#include <stdio.h>

#include "ui/base/keycodes/keyboard_codes.h"

namespace browser {

  struct AcceleratorMapping {
    ui::KeyboardCode keycode;
    bool shift_pressed;
    bool ctrl_pressed;
    bool alt_pressed;
    int command_id;
  };

  
  extern const AcceleratorMapping kAcceleratorMap[];

  
  extern const size_t kAcceleratorMapLength;
}

#endif  
