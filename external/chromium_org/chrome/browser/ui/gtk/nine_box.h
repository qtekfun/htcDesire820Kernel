// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_NINE_BOX_H_
#define CHROME_BROWSER_UI_GTK_NINE_BOX_H_

#include <gtk/gtk.h>

namespace gfx {
class Image;
}

class NineBox {
 public:
  
  
  NineBox(int top_left, int top, int top_right, int left, int center, int right,
          int bottom_left, int bottom, int bottom_right);

  
  
  NineBox(int image, int top_margin, int bottom_margin, int left_margin,
          int right_margin);
  ~NineBox();

  
  
  void RenderToWidget(GtkWidget* dst) const;

  
  void RenderToWidgetWithOpacity(GtkWidget* dst, double opacity) const;

  
  
  
  
  
  void ContourWidget(GtkWidget* widget) const;

 private:
  gfx::Image* images_[9];
  bool unref_images_on_destroy_;
};

#endif  
