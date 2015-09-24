// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_EV_BUBBLE_DECORATION_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_EV_BUBBLE_DECORATION_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "chrome/browser/ui/cocoa/location_bar/bubble_decoration.h"



class LocationIconDecoration;

class EVBubbleDecoration : public BubbleDecoration {
 public:
  EVBubbleDecoration(LocationIconDecoration* location_icon, NSFont* font);
  virtual ~EVBubbleDecoration();

  
  
  void SetFullLabel(NSString* full_label);

  
  
  NSPoint GetBubblePointInFrame(NSRect frame);

  
  virtual CGFloat GetWidthForSpace(CGFloat width);
  virtual bool IsDraggable();
  virtual NSPasteboard* GetDragPasteboard();
  virtual NSImage* GetDragImage();
  virtual NSRect GetDragImageFrame(NSRect frame);
  virtual bool OnMousePressed(NSRect frame);
  virtual bool AcceptsMousePress();

 private:
  
  scoped_nsobject<NSFont> font_;

  
  scoped_nsobject<NSString> full_label_;

  LocationIconDecoration* location_icon_;  

  DISALLOW_COPY_AND_ASSIGN(EVBubbleDecoration);
};

#endif  
