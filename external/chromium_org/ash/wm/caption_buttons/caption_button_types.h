// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_CAPTION_BUTTONS_CAPTION_BUTTON_TYPES_H_
#define ASH_WM_CAPTION_BUTTONS_CAPTION_BUTTON_TYPES_H_

namespace ash {

enum MaximizeBubbleFrameState {
  FRAME_STATE_NONE = 0,
  FRAME_STATE_FULL = 1,  
  FRAME_STATE_SNAP_LEFT = 2,
  FRAME_STATE_SNAP_RIGHT = 3
};

enum CaptionButtonIcon {
  CAPTION_BUTTON_ICON_MINIMIZE,
  CAPTION_BUTTON_ICON_MAXIMIZE_RESTORE,
  CAPTION_BUTTON_ICON_CLOSE,
  CAPTION_BUTTON_ICON_LEFT_SNAPPED,
  CAPTION_BUTTON_ICON_RIGHT_SNAPPED,
};

} 

#endif  