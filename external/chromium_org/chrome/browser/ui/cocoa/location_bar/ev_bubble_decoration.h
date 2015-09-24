// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_EV_BUBBLE_DECORATION_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_EV_BUBBLE_DECORATION_H_

#import <Cocoa/Cocoa.h>

#include "chrome/browser/ui/cocoa/location_bar/bubble_decoration.h"



class LocationIconDecoration;

class EVBubbleDecoration : public BubbleDecoration {
 public:
  explicit EVBubbleDecoration(LocationIconDecoration* location_icon);
  virtual ~EVBubbleDecoration();

  
  
  void SetFullLabel(NSString* full_label);

  
  
  NSPoint GetBubblePointInFrame(NSRect frame);

  
  virtual CGFloat GetWidthForSpace(CGFloat width) OVERRIDE;
  virtual bool IsDraggable() OVERRIDE;
  virtual NSPasteboard* GetDragPasteboard() OVERRIDE;
  virtual NSImage* GetDragImage() OVERRIDE;
  virtual NSRect GetDragImageFrame(NSRect frame) OVERRIDE;
  virtual bool OnMousePressed(NSRect frame) OVERRIDE;
  virtual bool AcceptsMousePress() OVERRIDE;

  
  virtual ui::NinePartImageIds GetBubbleImageIds() OVERRIDE;

 private:
  
  base::scoped_nsobject<NSString> full_label_;

  LocationIconDecoration* location_icon_;  

  DISALLOW_COPY_AND_ASSIGN(EVBubbleDecoration);
};

#endif  
