// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_GTK_GTK_HIG_CONSTANTS_H_
#define UI_BASE_GTK_GTK_HIG_CONSTANTS_H_

typedef struct _GdkColor GdkColor;

#define GDK_COLOR_RGB(r, g, b) {0, r * ::ui::kSkiaToGDKMultiplier,  \
        g * ::ui::kSkiaToGDKMultiplier, b * ::ui::kSkiaToGDKMultiplier}

namespace ui {

const int kSkiaToGDKMultiplier = 257;

const GdkColor kGdkWhite = GDK_COLOR_RGB(0xFF, 0xFF, 0xFF);
const GdkColor kGdkBlack = GDK_COLOR_RGB(0x00, 0x00, 0x00);
const GdkColor kGdkGray = GDK_COLOR_RGB(0x7F, 0x7F, 0x7F);


const int kControlSpacing = 6;

const int kLabelSpacing = 12;

const int kGroupIndent = 12;

const int kContentAreaBorder = 12;

const int kContentAreaSpacing = 18;

const int kFormControlSpacing = 10;

}  

#endif  
