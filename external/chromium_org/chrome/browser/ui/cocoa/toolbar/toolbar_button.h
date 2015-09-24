// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TOOLBAR_TOOLBAR_BUTTON_H_
#define CHROME_BROWSER_UI_COCOA_TOOLBAR_TOOLBAR_BUTTON_H_

#import <Cocoa/Cocoa.h>


@interface ToolbarButton : NSButton {
 @protected
  
  BOOL handleMiddleClick_;
}

@property(assign, nonatomic) BOOL handleMiddleClick;

@end

@interface ToolbarButton (ExposedForTesting)
- (BOOL)shouldHandleEvent:(NSEvent*)theEvent;
@end

#endif  
