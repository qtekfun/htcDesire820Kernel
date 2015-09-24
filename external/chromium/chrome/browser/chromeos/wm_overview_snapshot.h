// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_WM_OVERVIEW_SNAPSHOT_H_
#define CHROME_BROWSER_CHROMEOS_WM_OVERVIEW_SNAPSHOT_H_
#pragma once

#include "third_party/skia/include/core/SkBitmap.h"
#include "views/controls/image_view.h"
#include "views/view.h"
#include "views/widget/widget_gtk.h"

class Browser;

namespace chromeos {

class WmOverviewSnapshot : public views::WidgetGtk {
 public:
  WmOverviewSnapshot();
  void Init(const gfx::Size& size, Browser* browser, int index);

  void SetImage(const SkBitmap& image);

  void UpdateIndex(Browser* browser, int index);
  int index() const { return index_; }

  
  gfx::Size size() const {
    // TODO(beng): this should not be written as an accessor...
    return GetClientAreaScreenBounds().size();
  }

  
  
  bool configured_snapshot() const { return configured_snapshot_; }

  
  
  void reload_snapshot() { configured_snapshot_ = false; }

 private:
  
  views::ImageView* snapshot_view_;

  
  int index_;

  
  bool configured_snapshot_;

  DISALLOW_COPY_AND_ASSIGN(WmOverviewSnapshot);
};

}  

#endif  
