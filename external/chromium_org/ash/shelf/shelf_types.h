// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELF_SHELF_TYPES_H_
#define ASH_SHELF_SHELF_TYPES_H_

namespace ash {

enum ShelfAlignment {
  SHELF_ALIGNMENT_BOTTOM,
  SHELF_ALIGNMENT_LEFT,
  SHELF_ALIGNMENT_RIGHT,
  SHELF_ALIGNMENT_TOP,
};

enum ShelfAutoHideBehavior {
  
  SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS,

  
  SHELF_AUTO_HIDE_BEHAVIOR_NEVER,

  
  SHELF_AUTO_HIDE_ALWAYS_HIDDEN,
};

enum ShelfVisibilityState {
  
  SHELF_VISIBLE,

  
  SHELF_AUTO_HIDE,

  
  SHELF_HIDDEN,
};

enum ShelfAutoHideState {
  SHELF_AUTO_HIDE_SHOWN,
  SHELF_AUTO_HIDE_HIDDEN,
};

enum ShelfBackgroundType {
  
  SHELF_BACKGROUND_DEFAULT,

  
  SHELF_BACKGROUND_OVERLAP,

  
  SHELF_BACKGROUND_MAXIMIZED,
};

}  

#endif  
