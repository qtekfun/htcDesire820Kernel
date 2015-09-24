// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_DOCK_DOCK_TYPES_H_
#define ASH_WM_DOCK_DOCK_TYPES_H_

namespace ash {

namespace internal {

enum DockedAlignment {
  
  DOCKED_ALIGNMENT_NONE,

  
  DOCKED_ALIGNMENT_LEFT,

  
  DOCKED_ALIGNMENT_RIGHT,
};

enum DockedAction {
  DOCKED_ACTION_NONE,       
  DOCKED_ACTION_DOCK,       
  DOCKED_ACTION_UNDOCK,     
  DOCKED_ACTION_RESIZE,     
  DOCKED_ACTION_REORDER,    
  DOCKED_ACTION_EVICT,      
  DOCKED_ACTION_MAXIMIZE,   
  DOCKED_ACTION_MINIMIZE,   
  DOCKED_ACTION_RESTORE,    
  DOCKED_ACTION_CLOSE,      
  DOCKED_ACTION_COUNT,      
};

enum DockedActionSource {
  DOCKED_ACTION_SOURCE_UNKNOWN,
  DOCKED_ACTION_SOURCE_MOUSE,
  DOCKED_ACTION_SOURCE_TOUCH,

  
  DOCKED_ACTION_SOURCE_COUNT,
};

}  
}  

#endif  
