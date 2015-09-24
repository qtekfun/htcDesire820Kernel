// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_GTK_UTIL_H_
#define UI_GFX_GTK_UTIL_H_

#include <vector>

#include "base/time/time.h"
#include "ui/gfx/gfx_export.h"

typedef struct _GdkPixbuf GdkPixbuf;
typedef struct _GdkRegion GdkRegion;
typedef struct _GdkCursor GdkCursor;

class CommandLine;
class SkBitmap;

namespace gfx {

class Rect;

GFX_EXPORT void GtkInitFromCommandLine(const CommandLine& command_line);
GFX_EXPORT void GdkInitFromCommandLine(const CommandLine& command_line);

GFX_EXPORT GdkPixbuf* GdkPixbufFromSkBitmap(const SkBitmap& bitmap);

GFX_EXPORT void SubtractRectanglesFromRegion(GdkRegion* region,
                                             const std::vector<Rect>& cutouts);

GFX_EXPORT GdkCursor* GetCursor(int type);

GFX_EXPORT void InitRCStyles();

GFX_EXPORT base::TimeDelta GetCursorBlinkCycle();

}  

#endif  
