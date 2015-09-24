// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_ACCELERATOR_TABLE_H_
#define CHROME_BROWSER_UI_VIEWS_ACCELERATOR_TABLE_H_

#include <vector>

#include "chrome/browser/ui/host_desktop.h"
#include "chrome/browser/ui/views/chrome_views_export.h"
#include "ui/events/keycodes/keyboard_codes.h"

namespace ui {
class Accelerator;
}

namespace chrome {

struct AcceleratorMapping {
  ui::KeyboardCode keycode;
  int modifiers;
  int command_id;
};

CHROME_VIEWS_EXPORT std::vector<AcceleratorMapping> GetAcceleratorList();

CHROME_VIEWS_EXPORT bool GetAshAcceleratorForCommandId(
    int command_id,
    HostDesktopType host_desktop_type,
    ui::Accelerator* accelerator);

CHROME_VIEWS_EXPORT bool GetStandardAcceleratorForCommandId(
    int command_id,
    ui::Accelerator* accelerator);

}  

#endif  
