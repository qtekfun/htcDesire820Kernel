// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PANELS_PANEL_CONSTANTS_H_
#define CHROME_BROWSER_UI_PANELS_PANEL_CONSTANTS_H_

namespace panel {

static const int kTitlebarHeight = 36;

const int kPanelMinWidth = 80;
const int kPanelMinHeight = kTitlebarHeight + 10;

static const int kMinimizedPanelHeight = 4;

static const int kPanelAppIconSize = 32;

static const int kPanelButtonSize = 24;

const int kTitlebarLeftPadding = 10;

const int kTitlebarRightPadding = 6;

const int kIconAndTitlePadding = 11;

const int kTitleAndButtonPadding = 11;

static const int kButtonPadding = 5;

static const int kNumberOfTimesToFlashPanelForAttention = 30;

enum TitlebarButtonType {
  CLOSE_BUTTON,
  MINIMIZE_BUTTON,
  RESTORE_BUTTON
};

enum ClickModifier {
  NO_MODIFIER,
  APPLY_TO_ALL,  
};

enum ResizingSides {
  RESIZE_NONE = 0,
  RESIZE_TOP,
  RESIZE_TOP_RIGHT,
  RESIZE_RIGHT,
  RESIZE_BOTTOM_RIGHT,
  RESIZE_BOTTOM,
  RESIZE_BOTTOM_LEFT,
  RESIZE_LEFT,
  RESIZE_TOP_LEFT
};

enum Resizability {
  NOT_RESIZABLE = 0,
  RESIZABLE_TOP = 0x1,
  RESIZABLE_BOTTOM = 0x2,
  RESIZABLE_LEFT = 0x4,
  RESIZABLE_RIGHT = 0x8,
  RESIZABLE_TOP_LEFT = 0x10,
  RESIZABLE_TOP_RIGHT = 0x20,
  RESIZABLE_BOTTOM_LEFT = 0x40,
  RESIZABLE_BOTTOM_RIGHT = 0x80,
  RESIZABLE_EXCEPT_BOTTOM = RESIZABLE_TOP | RESIZABLE_LEFT | RESIZABLE_RIGHT |
      RESIZABLE_TOP_LEFT | RESIZABLE_TOP_RIGHT,
  RESIZABLE_ALL = RESIZABLE_TOP | RESIZABLE_BOTTOM | RESIZABLE_LEFT |
      RESIZABLE_RIGHT | RESIZABLE_TOP_LEFT | RESIZABLE_TOP_RIGHT |
      RESIZABLE_BOTTOM_LEFT | RESIZABLE_BOTTOM_RIGHT
};

enum CornerStyle {
  NOT_ROUNDED = 0,
  TOP_ROUNDED = 0x1,
  BOTTOM_ROUNDED = 0x2,
  ALL_ROUNDED = TOP_ROUNDED | BOTTOM_ROUNDED
};

}  

#endif  
