// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_ZOOM_DECORATION_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_ZOOM_DECORATION_H_

#import <Cocoa/Cocoa.h>

#include "base/basictypes.h"
#import "chrome/browser/ui/cocoa/browser/zoom_bubble_controller.h"
#include "chrome/browser/ui/cocoa/location_bar/image_decoration.h"

class LocationBarViewMac;
@class ZoomBubbleController;
class ZoomController;
class ZoomDecorationTest;

class ZoomDecoration : public ImageDecoration,
                       public ZoomBubbleControllerDelegate {
 public:
  explicit ZoomDecoration(LocationBarViewMac* owner);
  virtual ~ZoomDecoration();

  
  
  void Update(ZoomController* zoom_controller);

  
  
  void ShowBubble(BOOL auto_close);

  
  void CloseBubble();

 private:
  friend ZoomDecorationTest;

  NSPoint GetBubblePointInFrame(NSRect frame);
  bool IsAtDefaultZoom() const;
  bool ShouldShowDecoration() const;

  
  virtual bool AcceptsMousePress() OVERRIDE;
  virtual bool OnMousePressed(NSRect frame) OVERRIDE;
  virtual NSString* GetToolTip() OVERRIDE;

  
  virtual content::WebContents* GetWebContents() OVERRIDE;
  virtual void OnClose() OVERRIDE;

  
  LocationBarViewMac* owner_;

  
  ZoomBubbleController* bubble_;

  
  base::scoped_nsobject<NSString> tooltip_;

  DISALLOW_COPY_AND_ASSIGN(ZoomDecoration);
};

#endif  
