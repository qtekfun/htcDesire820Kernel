// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_ROUNDED_WINDOW_H_
#define CHROME_BROWSER_UI_GTK_ROUNDED_WINDOW_H_
#pragma once

#include <gtk/gtk.h>

namespace gtk_util {

enum RoundedBorders {
  ROUNDED_NONE = 0,
  ROUNDED_BOTTOM_LEFT = 1 << 0,
  ROUNDED_TOP_LEFT = 1 << 1,
  ROUNDED_TOP_RIGHT = 1 << 2,
  ROUNDED_TOP = (1 << 1) | (1 << 2),
  ROUNDED_BOTTOM_RIGHT = 1 << 3,
  ROUNDED_ALL = 0xF
};

enum BorderEdge {
  BORDER_NONE = 0,
  BORDER_LEFT = 1 << 0,
  BORDER_TOP = 1 << 1,
  BORDER_RIGHT = 1 << 2,
  BORDER_BOTTOM = 1 << 3,
  BORDER_ALL = 0xF
};

void ActAsRoundedWindow(
    GtkWidget* widget, const GdkColor& color, int corner_size,
    int rounded_edges, int drawn_borders);

void StopActingAsRoundedWindow(GtkWidget* widget);

bool IsActingAsRoundedWindow(GtkWidget* widget);

void SetRoundedWindowEdgesAndBorders(GtkWidget* widget,
                                     int corner_size,
                                     int rounded_edges,
                                     int drawn_borders);

void SetRoundedWindowBorderColor(GtkWidget* widget, GdkColor color);


}  

#endif  
