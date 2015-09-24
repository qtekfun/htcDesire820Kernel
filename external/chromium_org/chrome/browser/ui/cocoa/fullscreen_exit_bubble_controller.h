// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "chrome/browser/ui/fullscreen/fullscreen_exit_bubble_type.h"
#include "url/gurl.h"

@class BrowserWindowController;
class Browser;
@class GTMUILocalizerAndLayoutTweaker;

@interface FullscreenExitBubbleController :
    NSWindowController<NSTextViewDelegate, NSAnimationDelegate> {
 @private
  BrowserWindowController* owner_;  
  Browser* browser_; 
  GURL url_;
  FullscreenExitBubbleType bubbleType_;

 @protected
  IBOutlet NSTextField* exitLabelPlaceholder_;
  IBOutlet NSTextField* messageLabel_;
  IBOutlet NSButton* allowButton_;
  IBOutlet NSButton* denyButton_;
  IBOutlet GTMUILocalizerAndLayoutTweaker* tweaker_;

  
  
  
  
  base::scoped_nsobject<NSTextView> exitLabel_;

  base::scoped_nsobject<NSTimer> hideTimer_;
  base::scoped_nsobject<NSAnimation> hideAnimation_;
};

- (id)initWithOwner:(BrowserWindowController*)owner
            browser:(Browser*)browser
                url:(const GURL&)url
         bubbleType:(FullscreenExitBubbleType)bubbleType;

- (void)allow:(id)sender;
- (void)deny:(id)sender;

- (void)showWindow;
- (void)closeImmediately;

- (void)positionInWindowAtTop:(CGFloat)maxY width:(CGFloat)maxWidth;

@end
