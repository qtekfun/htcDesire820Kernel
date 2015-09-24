// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_X_X11_UTIL_H_
#define UI_GFX_X_X11_UTIL_H_

#include "base/basictypes.h"
#include "ui/gfx/gfx_export.h"

typedef unsigned long XID;
typedef struct _XImage XImage;
typedef struct _XGC *GC;
typedef struct _XDisplay XDisplay;

namespace gfx {

GFX_EXPORT XDisplay* GetXDisplay();

GFX_EXPORT int BitsPerPixelForPixmapDepth(XDisplay* display, int depth);

GFX_EXPORT void PutARGBImage(XDisplay* display,
                             void* visual, int depth,
                             XID pixmap, void* pixmap_gc,
                             const uint8* data,
                             int width, int height);

GFX_EXPORT void PutARGBImage(XDisplay* display,
                             void* visual, int depth,
                             XID pixmap, void* pixmap_gc,
                             const uint8* data,
                             int data_width, int data_height,
                             int src_x, int src_y,
                             int dst_x, int dst_y,
                             int copy_width, int copy_height);

}  

#endif  

