// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_GDK_COMPAT_H_
#define UI_GFX_GDK_COMPAT_H_

#include <gtk/gtk.h>
#include <gdk/gdkx.h>


#if !GTK_CHECK_VERSION(2, 24, 0)
inline GdkWindow* gdk_x11_window_lookup_for_display(GdkDisplay* display,
                                                    Window window) {
  return static_cast<GdkWindow*>(gdk_xid_table_lookup_for_display(display,
                                                                  window));
}
#endif

#endif  
