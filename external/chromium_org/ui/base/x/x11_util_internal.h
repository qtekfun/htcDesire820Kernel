// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_X_X11_UTIL_INTERNAL_H_
#define UI_BASE_X_X11_UTIL_INTERNAL_H_


extern "C" {
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>
#include <X11/extensions/Xrender.h>
}

#include "ui/base/ui_export.h"

namespace ui {

XRenderPictFormat* GetRenderARGB32Format(Display* dpy);

UI_EXPORT XRenderPictFormat* GetRenderVisualFormat(Display* dpy,
                                                   Visual* visual);

UI_EXPORT void SetX11ErrorHandlers(XErrorHandler error_handler,
                                   XIOErrorHandler io_error_handler);

UI_EXPORT void LogErrorEventDescription(Display* dpy,
                                        const XErrorEvent& error_event);

}  

#endif  
