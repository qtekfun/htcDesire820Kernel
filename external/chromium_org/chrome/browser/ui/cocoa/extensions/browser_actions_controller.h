// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_EXTENSIONS_BROWSER_ACTIONS_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_EXTENSIONS_BROWSER_ACTIONS_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#import "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"

class Browser;
@class BrowserActionButton;
@class BrowserActionsContainerView;
@class ExtensionPopupController;
class ExtensionToolbarModel;
class ExtensionServiceObserverBridge;
@class MenuButton;
class Profile;

namespace extensions {
class Extension;
}

extern NSString* const kBrowserActionVisibilityChangedNotification;

@interface BrowserActionsController : NSObject<NSMenuDelegate> {
 @private
  
  Browser* browser_;

  
  BrowserActionsContainerView* containerView_;

  
  Profile* profile_;

  
  ExtensionToolbarModel* toolbarModel_;

  
  scoped_ptr<ExtensionServiceObserverBridge> observer_;

  
  
  
  base::scoped_nsobject<NSMutableDictionary> buttons_;

  
  base::scoped_nsobject<NSMutableArray> hiddenButtons_;

  
  base::scoped_nsobject<NSViewAnimation> chevronAnimation_;

  
  base::scoped_nsobject<MenuButton> chevronMenuButton_;

  
  base::scoped_nsobject<NSMenu> overflowMenu_;
}

@property(readonly, nonatomic) BrowserActionsContainerView* containerView;

- (id)initWithBrowser:(Browser*)browser
        containerView:(BrowserActionsContainerView*)container;

- (void)update;

- (NSUInteger)buttonCount;

- (NSUInteger)visibleButtonCount;

- (void)resizeContainerAndAnimate:(BOOL)animate;

- (NSView*)browserActionViewForExtension:(
    const extensions::Extension*)extension;

- (CGFloat)savedWidth;

- (NSPoint)popupPointForBrowserAction:(const extensions::Extension*)extension;

- (BOOL)chevronIsHidden;

@end  

@interface BrowserActionsController(TestingAPI)
- (NSButton*)buttonWithIndex:(NSUInteger)index;
@end

#endif  
