// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#include "base/string16.h"
#include "ui/gfx/point.h"

@class BrowserWindowController;
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

  
  FindBarBridge* findBarBridge_;  

  
  BrowserWindowController* browserWindowController_;  

  scoped_nsobject<FocusTracker> focusTracker_;

  
  
  
  
  
  scoped_nsobject<NSViewAnimation> showHideAnimation_;

  
  
  
  scoped_nsobject<NSViewAnimation> moveAnimation_;

  
  BOOL suppressPboardUpdateActions_;

  
  CGFloat maxY_;
};

- (id)init;

- (void)setFindBarBridge:(FindBarBridge*)findBar;
- (void)setBrowserWindowController:(BrowserWindowController*)controller;

- (IBAction)close:(id)sender;

- (IBAction)nextResult:(id)sender;

- (IBAction)previousResult:(id)sender;

- (void)positionFindBarViewAtMaxY:(CGFloat)maxY maxWidth:(CGFloat)maxWidth;

- (void)showFindBar:(BOOL)animate;
- (void)hideFindBar:(BOOL)animate;
- (void)stopAnimation;
- (void)setFocusAndSelection;
- (void)restoreSavedFocus;
- (void)setFindText:(NSString*)findText;

- (void)clearResults:(const FindNotificationDetails&)results;
- (void)updateUIForFindResult:(const FindNotificationDetails&)results
                     withText:(const string16&)findText;
- (BOOL)isFindBarVisible;
- (BOOL)isFindBarAnimating;

- (gfx::Point)findBarWindowPosition;

@end
