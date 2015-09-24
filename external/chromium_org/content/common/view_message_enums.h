// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_VIEW_MESSAGES_ENUMS_H_
#define CONTENT_COMMON_VIEW_MESSAGES_ENUMS_H_

#include "ipc/ipc_message_macros.h"

struct ViewHostMsg_UpdateRect_Flags {
  enum {
    IS_RESIZE_ACK = 1 << 0,
    IS_RESTORE_ACK = 1 << 1,
    IS_REPAINT_ACK = 1 << 2,
  };
  static bool is_resize_ack(int flags) {
    return (flags & IS_RESIZE_ACK) != 0;
  }
  static bool is_restore_ack(int flags) {
    return (flags & IS_RESTORE_ACK) != 0;
  }
  static bool is_repaint_ack(int flags) {
    return (flags & IS_REPAINT_ACK) != 0;
  }
};

struct ViewMsg_Navigate_Type {
 public:
  enum Value {
    
    RELOAD,

    
    RELOAD_IGNORING_CACHE,

    
    RELOAD_ORIGINAL_REQUEST_URL,

    
    
    
    
    
    RESTORE,

    
    RESTORE_WITH_POST,

    
    NORMAL
  };
};

enum AccessibilityMode {
  
  
  AccessibilityModeOff,

  
  
  
  AccessibilityModeEditableTextOnly,

  
  
  AccessibilityModeComplete,
};

#endif  
