// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_GTK_WINDOW_UTIL_H_
#define CHROME_BROWSER_UI_GTK_GTK_WINDOW_UTIL_H_

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "ui/gfx/rect.h"

namespace content {
class WebContents;
}

namespace ui{
class BaseWindow;
}

namespace gtk_window_util {

extern const int kFrameBorderThickness;
extern const int kResizeAreaCornerSize;

void DoCut(GtkWindow* window, content::WebContents* web_contents);
void DoCopy(GtkWindow* window, content::WebContents* web_contents);
void DoPaste(GtkWindow* window, content::WebContents* web_contents);

void DisableResizeGrip(GtkWindow* window);

GdkCursorType GdkWindowEdgeToGdkCursorType(GdkWindowEdge edge);

bool BoundsMatchMonitorSize(GtkWindow* window, gfx::Rect bounds);

bool HandleTitleBarLeftMousePress(GtkWindow* window,
                                  const gfx::Rect& bounds,
                                  GdkEventButton* event);

void UnMaximize(GtkWindow* window,
                const gfx::Rect& bounds,
                const gfx::Rect& restored_bounds);

void SetWindowCustomClass(GtkWindow* window, const std::string& wmclass);

void SetWindowSize(GtkWindow* window, const gfx::Size& size);

void UpdateWindowPosition(ui::BaseWindow* window,
                          gfx::Rect* bounds,
                          gfx::Rect* restored_bounds);

bool GetWindowEdge(const gfx::Size& window_size,
                   int top_edge_inset,
                   int x,
                   int y,
                   GdkWindowEdge* edge);

}  

#endif  

