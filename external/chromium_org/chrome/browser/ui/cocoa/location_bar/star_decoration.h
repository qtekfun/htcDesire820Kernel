// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_STAR_DECORATION_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_STAR_DECORATION_H_

#import <Cocoa/Cocoa.h>

#include "chrome/browser/ui/cocoa/location_bar/image_decoration.h"

class CommandUpdater;


class StarDecoration : public ImageDecoration {
 public:
  explicit StarDecoration(CommandUpdater* command_updater);
  virtual ~StarDecoration();

  
  void SetStarred(bool starred);

  
  bool starred() const { return starred_; }

  
  
  NSPoint GetBubblePointInFrame(NSRect frame);

  
  virtual bool AcceptsMousePress() OVERRIDE;
  virtual bool OnMousePressed(NSRect frame) OVERRIDE;
  virtual NSString* GetToolTip() OVERRIDE;

 private:
  
  CommandUpdater* command_updater_;  

  
  base::scoped_nsobject<NSString> tooltip_;

  
  bool starred_;

  DISALLOW_COPY_AND_ASSIGN(StarDecoration);
};

#endif  
