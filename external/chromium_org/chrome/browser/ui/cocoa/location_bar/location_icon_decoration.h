// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_LOCATION_ICON_DECORATION_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_LOCATION_ICON_DECORATION_H_

#import <Cocoa/Cocoa.h>

#include "chrome/browser/ui/cocoa/location_bar/image_decoration.h"

class LocationBarViewMac;


class LocationIconDecoration : public ImageDecoration {
 public:
  explicit LocationIconDecoration(LocationBarViewMac* owner);
  virtual ~LocationIconDecoration();

  
  virtual bool IsDraggable() OVERRIDE;
  virtual NSPasteboard* GetDragPasteboard() OVERRIDE;
  virtual NSImage* GetDragImage() OVERRIDE;
  virtual NSRect GetDragImageFrame(NSRect frame) OVERRIDE;

  
  
  NSPoint GetBubblePointInFrame(NSRect frame);

  
  virtual bool OnMousePressed(NSRect frame) OVERRIDE;
  virtual bool AcceptsMousePress() OVERRIDE;
  virtual NSString* GetToolTip() OVERRIDE;

 private:
  NSRect drag_frame_;
  
  LocationBarViewMac* owner_;

  DISALLOW_COPY_AND_ASSIGN(LocationIconDecoration);
};

#endif  
