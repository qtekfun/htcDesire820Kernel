// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_WM_OVERVIEW_FAVICON_H_
#define CHROME_BROWSER_CHROMEOS_WM_OVERVIEW_FAVICON_H_
#pragma once

#include "views/widget/widget_gtk.h"

class SkBitmap;

namespace views {
class ImageView;
}

namespace chromeos {

class WmOverviewSnapshot;

class WmOverviewFavicon : public views::WidgetGtk {
 public:
  static const int kIconSize;

  WmOverviewFavicon();

  
  void Init(WmOverviewSnapshot* snapshot);

  
  
  void SetFavicon(const SkBitmap& image);

 private:
  
  views::ImageView* favicon_view_;

  DISALLOW_COPY_AND_ASSIGN(WmOverviewFavicon);
};

}  

#endif  
