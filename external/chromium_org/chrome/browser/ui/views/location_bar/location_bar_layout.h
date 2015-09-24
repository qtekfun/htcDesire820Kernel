// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_LOCATION_BAR_LAYOUT_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_LOCATION_BAR_LAYOUT_H_

#include "base/memory/scoped_vector.h"

namespace gfx {
class Rect;
}

namespace views {
class View;
}

struct LocationBarDecoration;

class LocationBarLayout {

 public:
  enum Position {
    LEFT_EDGE = 0,
    RIGHT_EDGE,
  };

  LocationBarLayout(Position position, int item_edit_padding);
  virtual ~LocationBarLayout();

  
  
  
  
  
  
  
  
  
  
  
  
  void AddDecoration(int y,
                     int height,
                     bool auto_collapse,
                     double max_fraction,
                     int edge_item_padding,
                     int item_padding,
                     int builtin_padding,
                     views::View* view);

  
  void AddDecoration(int y, int height, int builtin_padding, views::View* view);

  
  
  
  void LayoutPass1(int* entry_width);

  
  
  
  void LayoutPass2(int* entry_width);

  
  
  
  
  
  
  void LayoutPass3(gfx::Rect* bounds, int* available_width);

 private:
  typedef ScopedVector<LocationBarDecoration> Decorations;

  
  
  Position position_;

  
  int item_edit_padding_;

  
  Decorations decorations_;

  DISALLOW_COPY_AND_ASSIGN(LocationBarLayout);
};

#endif  
