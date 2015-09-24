// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_IMAGE_DECORATION_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_IMAGE_DECORATION_H_
#pragma once

#import "base/memory/scoped_nsobject.h"
#include "chrome/browser/ui/cocoa/location_bar/location_bar_decoration.h"


class ImageDecoration : public LocationBarDecoration {
 public:
  ImageDecoration();
  virtual ~ImageDecoration();

  NSImage* GetImage();
  void SetImage(NSImage* image);

  
  NSRect GetDrawRectInFrame(NSRect frame);

  
  virtual CGFloat GetWidthForSpace(CGFloat width);
  virtual void DrawInFrame(NSRect frame, NSView* control_view);

 private:
  scoped_nsobject<NSImage> image_;

  DISALLOW_COPY_AND_ASSIGN(ImageDecoration);
};

#endif  