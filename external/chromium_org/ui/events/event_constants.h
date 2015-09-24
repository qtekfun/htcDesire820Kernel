// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_EVENT_CONSTANTS_H_
#define UI_EVENTS_EVENT_CONSTANTS_H_

namespace ui {

enum EventType {
  ET_UNKNOWN = 0,
  ET_MOUSE_PRESSED,
  ET_MOUSE_DRAGGED,
  ET_MOUSE_RELEASED,
  ET_MOUSE_MOVED,
  ET_MOUSE_ENTERED,
  ET_MOUSE_EXITED,
  ET_KEY_PRESSED,
  ET_KEY_RELEASED,
  ET_MOUSEWHEEL,
  ET_MOUSE_CAPTURE_CHANGED,  
  ET_TOUCH_RELEASED,
  ET_TOUCH_PRESSED,
  ET_TOUCH_MOVED,
  ET_TOUCH_STATIONARY,
  ET_TOUCH_CANCELLED,
  ET_DROP_TARGET_EVENT,
  ET_TRANSLATED_KEY_PRESS,
  ET_TRANSLATED_KEY_RELEASE,

  
  ET_GESTURE_SCROLL_BEGIN,
  ET_GESTURE_SCROLL_END,
  ET_GESTURE_SCROLL_UPDATE,
  ET_GESTURE_TAP,
  ET_GESTURE_TAP_DOWN,
  ET_GESTURE_TAP_CANCEL,
  ET_GESTURE_BEGIN,  
  ET_GESTURE_END,    
  ET_GESTURE_TWO_FINGER_TAP,
  ET_GESTURE_PINCH_BEGIN,
  ET_GESTURE_PINCH_END,
  ET_GESTURE_PINCH_UPDATE,
  ET_GESTURE_LONG_PRESS,
  ET_GESTURE_LONG_TAP,
  
  
  ET_GESTURE_MULTIFINGER_SWIPE,
  ET_GESTURE_SHOW_PRESS,

  
  
  ET_SCROLL,
  ET_SCROLL_FLING_START,
  ET_SCROLL_FLING_CANCEL,

  
  
  ET_CANCEL_MODE,

  
  
  ET_UMA_DATA,

  
  
  ET_LAST
};

enum EventFlags {
  EF_NONE                = 0,       
  EF_CAPS_LOCK_DOWN      = 1 << 0,
  EF_SHIFT_DOWN          = 1 << 1,
  EF_CONTROL_DOWN        = 1 << 2,
  EF_ALT_DOWN            = 1 << 3,
  EF_LEFT_MOUSE_BUTTON   = 1 << 4,
  EF_MIDDLE_MOUSE_BUTTON = 1 << 5,
  EF_RIGHT_MOUSE_BUTTON  = 1 << 6,
  EF_COMMAND_DOWN        = 1 << 7,  
  EF_EXTENDED            = 1 << 8,  
  EF_IS_SYNTHESIZED      = 1 << 9,
  EF_ALTGR_DOWN          = 1 << 10,
};

enum MouseEventFlags {
  EF_IS_DOUBLE_CLICK    = 1 << 16,
  EF_IS_TRIPLE_CLICK    = 1 << 17,
  EF_IS_NON_CLIENT      = 1 << 18,
  EF_FROM_TOUCH         = 1 << 19,  
                                    
};

enum EventResult {
  ER_UNHANDLED = 0,       
                          
  ER_HANDLED   = 1 << 0,  
                          
  ER_CONSUMED  = 1 << 1,  
                          
};

enum EventPhase {
  EP_PREDISPATCH,
  EP_PRETARGET,
  EP_TARGET,
  EP_POSTTARGET,
  EP_POSTDISPATCH
};

}  

#endif  
