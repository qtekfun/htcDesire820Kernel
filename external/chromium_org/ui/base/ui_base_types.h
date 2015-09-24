// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_UI_BASE_TYPES_H_
#define UI_BASE_UI_BASE_TYPES_H_

#include "ui/base/ui_export.h"

namespace ui {

class Event;

// Window "show" state.  These values are written to disk so should not be
enum WindowShowState {
  
  SHOW_STATE_DEFAULT    = 0,
  SHOW_STATE_NORMAL     = 1,
  SHOW_STATE_MINIMIZED  = 2,
  SHOW_STATE_MAXIMIZED  = 3,
  SHOW_STATE_INACTIVE   = 4,  
  SHOW_STATE_FULLSCREEN = 5,
  SHOW_STATE_DETACHED   = 6,  
  SHOW_STATE_END        = 7   
};

enum DialogButton {
  DIALOG_BUTTON_NONE   = 0,
  DIALOG_BUTTON_OK     = 1,
  DIALOG_BUTTON_CANCEL = 2,
};

enum ModalType {
  MODAL_TYPE_NONE   = 0,  
  MODAL_TYPE_WINDOW = 1,  
  MODAL_TYPE_CHILD  = 2,  
  MODAL_TYPE_SYSTEM = 3   
};

enum MenuSourceType {
  MENU_SOURCE_NONE            = 0,
  MENU_SOURCE_MOUSE           = 1,
  MENU_SOURCE_KEYBOARD        = 2,
  MENU_SOURCE_TOUCH           = 3,
  MENU_SOURCE_TOUCH_EDIT_MENU = 4,
};

UI_EXPORT MenuSourceType GetMenuSourceTypeForEvent(const ui::Event& event);

}  

#endif  
