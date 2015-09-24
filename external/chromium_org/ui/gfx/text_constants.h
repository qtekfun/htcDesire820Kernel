// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_TEXT_CONSTANTS_H_
#define UI_GFX_TEXT_CONSTANTS_H_

namespace gfx {

enum BreakType {
  
  CHARACTER_BREAK = 0,
  
  WORD_BREAK,
  
  LINE_BREAK,
};

enum HorizontalAlignment {
  
  ALIGN_LEFT = 0,
  
  ALIGN_CENTER,
  
  ALIGN_RIGHT,
};

enum DirectionalityMode {
  
  DIRECTIONALITY_FROM_TEXT = 0,
  
  DIRECTIONALITY_FROM_UI,
  
  DIRECTIONALITY_FORCE_LTR,
  
  DIRECTIONALITY_FORCE_RTL,
};

enum TextStyle {
  BOLD = 0,
  ITALIC,
  STRIKE,
  DIAGONAL_STRIKE,
  UNDERLINE,
  NUM_TEXT_STYLES,
};

}  

#endif  
