// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_APP_LIST_POSITIONER_H_
#define CHROME_BROWSER_UI_APP_LIST_APP_LIST_POSITIONER_H_

#include "ui/gfx/display.h"
#include "ui/gfx/size.h"

namespace gfx {
class Point;
class Rect;
}

class AppListPositioner {
 public:
  
  enum ScreenEdge {
    SCREEN_EDGE_UNKNOWN,
    SCREEN_EDGE_LEFT,
    SCREEN_EDGE_RIGHT,
    SCREEN_EDGE_TOP,
    SCREEN_EDGE_BOTTOM
  };

  
  enum ScreenCorner {
    SCREEN_CORNER_TOP_LEFT,
    SCREEN_CORNER_TOP_RIGHT,
    SCREEN_CORNER_BOTTOM_LEFT,
    SCREEN_CORNER_BOTTOM_RIGHT
  };

  
  
  
  
  AppListPositioner(const gfx::Display& display,
                    const gfx::Size& window_size,
                    int min_distance_from_edge);

  
  
  
  void WorkAreaSubtract(const gfx::Rect& rect);

  
  void WorkAreaInset(int left, int top, int right, int bottom);

  
  
  
  
  
  gfx::Point GetAnchorPointForScreenCorner(ScreenCorner corner) const;

  
  
  
  
  
  gfx::Point GetAnchorPointForShelfCorner(ScreenEdge shelf_edge) const;

  
  
  
  
  gfx::Point GetAnchorPointForShelfCenter(ScreenEdge shelf_edge) const;

  
  
  
  
  
  gfx::Point GetAnchorPointForShelfCursor(ScreenEdge shelf_edge,
                                          const gfx::Point& cursor) const;

  
  
  
  ScreenEdge GetShelfEdge(const gfx::Rect& shelf_rect) const;

  
  
  
  
  int GetCursorDistanceFromShelf(ScreenEdge shelf_edge,
                                 const gfx::Point& cursor) const;

 private:
  
  
  gfx::Point ClampAnchorPoint(gfx::Point anchor) const;

  gfx::Display display_;

  
  gfx::Size window_size_;

  
  
  int min_distance_from_edge_;
};

#endif  
