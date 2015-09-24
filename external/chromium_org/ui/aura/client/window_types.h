// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_WINDOW_TYPES_H_
#define UI_AURA_WINDOW_TYPES_H_

namespace aura {
namespace client {

enum WindowType {
  WINDOW_TYPE_UNKNOWN = 0,

  
  WINDOW_TYPE_NORMAL,

  
  WINDOW_TYPE_POPUP,

  
  WINDOW_TYPE_CONTROL,

  
  WINDOW_TYPE_PANEL,

  WINDOW_TYPE_MENU,
  WINDOW_TYPE_TOOLTIP,
};

}  
}  

#endif  
