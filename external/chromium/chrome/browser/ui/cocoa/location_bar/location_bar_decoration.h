// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_LOCATION_BAR_DECORATION_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_LOCATION_BAR_DECORATION_H_
#pragma once

#import <Cocoa/Cocoa.h>

#import "base/basictypes.h"



class LocationBarDecoration {
 public:
  LocationBarDecoration()
      : visible_(false) {
  }
  virtual ~LocationBarDecoration() {}

  
  virtual bool IsVisible() const;
  virtual void SetVisible(bool visible);

  
  
  
  virtual CGFloat GetWidthForSpace(CGFloat width);

  
  
  virtual void DrawInFrame(NSRect frame, NSView* control_view);

  
  virtual NSString* GetToolTip();

  
  
  
  
  
  virtual bool AcceptsMousePress();

  
  virtual bool IsDraggable();

  
  virtual NSImage* GetDragImage();

  
  
  
  
  virtual NSRect GetDragImageFrame(NSRect frame);

  
  virtual NSPasteboard* GetDragPasteboard();

  
  
  virtual bool OnMousePressed(NSRect frame);

  
  virtual NSMenu* GetMenu();

  
  
  static const CGFloat kOmittedWidth;

 private:
  bool visible_;

  DISALLOW_COPY_AND_ASSIGN(LocationBarDecoration);
};

#endif  
