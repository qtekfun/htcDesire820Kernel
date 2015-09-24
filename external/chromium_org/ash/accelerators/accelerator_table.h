// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_ACCELERATORS_ACCELERATOR_TABLE_H_
#define ASH_ACCELERATORS_ACCELERATOR_TABLE_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "ui/events/event_constants.h"
#include "ui/events/keycodes/keyboard_codes.h"

namespace ash {

enum AcceleratorAction {
  ACCESSIBLE_FOCUS_NEXT,
  ACCESSIBLE_FOCUS_PREVIOUS,
  BRIGHTNESS_DOWN,
  BRIGHTNESS_UP,
  CYCLE_BACKWARD_MRU,
  CYCLE_FORWARD_MRU,
  CYCLE_LINEAR,
  DEBUG_TOGGLE_DEVICE_SCALE_FACTOR,
  DEBUG_TOGGLE_SHOW_DEBUG_BORDERS,
  DEBUG_TOGGLE_SHOW_FPS_COUNTER,
  DEBUG_TOGGLE_SHOW_PAINT_RECTS,
  DISABLE_CAPS_LOCK,
  EXIT,
  FOCUS_LAUNCHER,
  FOCUS_NEXT_PANE,
  FOCUS_PREVIOUS_PANE,
  KEYBOARD_BRIGHTNESS_DOWN,
  KEYBOARD_BRIGHTNESS_UP,
  LAUNCH_APP_0,
  LAUNCH_APP_1,
  LAUNCH_APP_2,
  LAUNCH_APP_3,
  LAUNCH_APP_4,
  LAUNCH_APP_5,
  LAUNCH_APP_6,
  LAUNCH_APP_7,
  LAUNCH_LAST_APP,
  LOCK_PRESSED,
  LOCK_RELEASED,
  MAGNIFY_SCREEN_ZOOM_IN,
  MAGNIFY_SCREEN_ZOOM_OUT,
  MEDIA_NEXT_TRACK,
  MEDIA_PLAY_PAUSE,
  MEDIA_PREV_TRACK,
  NEW_INCOGNITO_WINDOW,
  NEW_TAB,
  NEW_WINDOW,
  NEXT_IME,
  OPEN_FEEDBACK_PAGE,
  POWER_PRESSED,
  POWER_RELEASED,
  PREVIOUS_IME,
  PRINT_LAYER_HIERARCHY,
  PRINT_UI_HIERARCHIES,
  PRINT_VIEW_HIERARCHY,
  PRINT_WINDOW_HIERARCHY,
  RESTORE_TAB,
  ROTATE_SCREEN,
  ROTATE_WINDOW,
  SCALE_UI_DOWN,
  SCALE_UI_RESET,
  SCALE_UI_UP,
  SHOW_KEYBOARD_OVERLAY,
  SHOW_MESSAGE_CENTER_BUBBLE,
  SHOW_OAK,
  SHOW_SYSTEM_TRAY_BUBBLE,
  SHOW_TASK_MANAGER,
  SILENCE_SPOKEN_FEEDBACK,
  SWAP_PRIMARY_DISPLAY,
  SWITCH_IME,  
  TAKE_PARTIAL_SCREENSHOT,
  TAKE_SCREENSHOT,
  TOGGLE_APP_LIST,
  TOGGLE_CAPS_LOCK,
  TOGGLE_CAPS_LOCK_BY_ALT_LWIN,
  TOGGLE_DESKTOP_BACKGROUND_MODE,
  TOGGLE_FULLSCREEN,
  TOGGLE_MAXIMIZED,
  TOGGLE_ROOT_WINDOW_FULL_SCREEN,
  TOGGLE_SPOKEN_FEEDBACK,
  TOGGLE_WIFI,
  TOUCH_HUD_CLEAR,
  TOUCH_HUD_MODE_CHANGE,
  TOUCH_HUD_PROJECTION_TOGGLE,
  VOLUME_DOWN,
  VOLUME_MUTE,
  VOLUME_UP,
  WINDOW_MINIMIZE,
  WINDOW_POSITION_CENTER,
  WINDOW_SNAP_LEFT,
  WINDOW_SNAP_RIGHT,
#if defined(OS_CHROMEOS)
  ADD_REMOVE_DISPLAY,
  TOGGLE_MIRROR_MODE,
  DISABLE_GPU_WATCHDOG,
  LOCK_SCREEN,
  OPEN_CROSH,
  OPEN_FILE_MANAGER,
  SWITCH_TO_NEXT_USER,
  SWITCH_TO_PREVIOUS_USER,
#endif
};

struct AcceleratorData {
  bool trigger_on_press;
  ui::KeyboardCode keycode;
  int modifiers;
  AcceleratorAction action;
};

ASH_EXPORT extern const AcceleratorData kAcceleratorData[];
ASH_EXPORT extern const size_t kAcceleratorDataLength;

#if !defined(NDEBUG)
ASH_EXPORT extern const AcceleratorData kDesktopAcceleratorData[];
ASH_EXPORT extern const size_t kDesktopAcceleratorDataLength;
#endif

ASH_EXPORT extern const AcceleratorData kDebugAcceleratorData[];
ASH_EXPORT extern const size_t kDebugAcceleratorDataLength;

ASH_EXPORT extern const AcceleratorAction kReservedActions[];
ASH_EXPORT extern const size_t kReservedActionsLength;

ASH_EXPORT extern const AcceleratorAction kReservedDebugActions[];
ASH_EXPORT extern const size_t kReservedDebugActionsLength;

ASH_EXPORT extern const AcceleratorAction kActionsAllowedAtLoginOrLockScreen[];
ASH_EXPORT extern const size_t kActionsAllowedAtLoginOrLockScreenLength;

ASH_EXPORT extern const AcceleratorAction kActionsAllowedAtLockScreen[];
ASH_EXPORT extern const size_t kActionsAllowedAtLockScreenLength;

ASH_EXPORT extern const AcceleratorAction kActionsAllowedAtModalWindow[];
ASH_EXPORT extern const size_t kActionsAllowedAtModalWindowLength;

ASH_EXPORT extern const AcceleratorAction kNonrepeatableActions[];
ASH_EXPORT extern const size_t kNonrepeatableActionsLength;

ASH_EXPORT extern const AcceleratorAction kActionsAllowedInAppMode[];
ASH_EXPORT extern const size_t kActionsAllowedInAppModeLength;

ASH_EXPORT extern const AcceleratorAction kActionsNeedingWindow[];
ASH_EXPORT extern const size_t kActionsNeedingWindowLength;

}  

#endif  
