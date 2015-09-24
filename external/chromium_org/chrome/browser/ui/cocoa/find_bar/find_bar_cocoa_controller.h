// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/strings/string16.h"
#include "ui/gfx/point.h"

class Browser;
class FindBarBridge;
@class FindBarTextField;
class FindNotificationDetails;
@class FocusTracker;


@interface FindBarCocoaController : NSViewController {
 @private
  IBOutlet NSView* findBarView_;
  IBOutlet FindBarTextField* findText_;
  IBOutlet NSButton* nextButton_;
  IBOutlet NSButton* previousButton_;
  IBOutlet NSButton* closeButton_;

  
  FindBarBridge* findBarBridge_;  

  Browser* browser_;

  base::scoped_nsobject<FocusTracker> focusTracker_;

  
  
  
  
  
  base::scoped_nsobject<NSViewAnimation> showHideAnimation_;

  
  
  
  base::scoped_nsobject<NSViewAnimation> moveAnimation_;

  
  BOOL suppressPboardUpdateActions_;

  
  CGFloat maxY_;

  
  CGFloat defaultWidth_;
};

@property (readonly, nonatomic) NSView* findBarView;

- (id)initWithBrowser:(Browser*)browser;

- (void)setFindBarBridge:(FindBarBridge*)findBar;

- (IBAction)close:(id)sender;

- (IBAction)nextResult:(id)sender;

- (IBAction)previousResult:(id)sender;

- (void)positionFindBarViewAtMaxY:(CGFloat)maxY maxWidth:(CGFloat)maxWidth;

- (void)showFindBar:(BOOL)animate;
- (void)hideFindBar:(BOOL)animate;
- (void)stopAnimation;
- (void)setFocusAndSelection;
- (void)restoreSavedFocus;
- (void)setFindText:(NSString*)findText
      selectedRange:(const NSRange&)selectedRange;
- (NSString*)findText;
- (NSRange)selectedRange;
- (NSString*)matchCountText;
- (void)updateFindBarForChangedWebContents;

- (void)clearResults:(const FindNotificationDetails&)results;
- (void)updateUIForFindResult:(const FindNotificationDetails&)results
                     withText:(const base::string16&)findText;
- (BOOL)isFindBarVisible;
- (BOOL)isFindBarAnimating;

- (gfx::Point)findBarWindowPosition;

- (int)findBarWidth;

@end
