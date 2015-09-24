// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_ACCESSIBILITY_ACCESSIBLE_VIEW_STATE_H_
#define UI_BASE_ACCESSIBILITY_ACCESSIBLE_VIEW_STATE_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/strings/string16.h"
#include "ui/base/accessibility/accessibility_types.h"
#include "ui/base/ui_export.h"

namespace ui {

struct UI_EXPORT AccessibleViewState {
 public:
  AccessibleViewState();
  ~AccessibleViewState();

  
  AccessibilityTypes::Role role;

  
  
  AccessibilityTypes::State state;

  
  base::string16 name;

  
  base::string16 value;

  
  base::string16 default_action;

  
  base::string16 keyboard_shortcut;

  
  
  int selection_start;
  int selection_end;

  
  
  
  int index;
  int count;

  
  
  
  
  
  
  
  
  
  base::Callback<void(const base::string16&)> set_value_callback;
};

}  

#endif  
