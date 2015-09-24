// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_EXTENSIONS_BROWSER_ACTIONS_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_EXTENSIONS_BROWSER_ACTIONS_CONTROLLER_H_
#pragma once

#import <Cocoa/Cocoa.h>

#import "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"

class Browser;
@class BrowserActionButton;
@class BrowserActionsContainerView;
class Extension;
@class ExtensionPopupController;
class ExtensionToolbarModel;
class ExtensionServiceObserverBridge;
@class MenuButton;
class PrefService;
class Profile;

extern NSString* const kBrowserActionVisibilityChangedNotification;

@interface BrowserActionsController : NSObject {
 @private
  
  Browser* browser_;

  
  BrowserActionsContainerView* containerView_;

  
  Profile* profile_;

  
  ExtensionToolbarModel* toolbarModel_;

  
  scoped_ptr<ExtensionServiceObserverBridge> observer_;

  
  
  
  scoped_nsobject<NSMutableDictionary> buttons_;

  
  scoped_nsobject<NSMutableArray> hiddenButtons_;

  
  scoped_nsobject<NSViewAnimation> chevronAnimation_;

  
  scoped_nsobject<MenuButton> chevronMenuButton_;

  
  scoped_nsobject<NSMenu> overflowMenu_;
}

@property(readonly, nonatomic) BrowserActionsContainerView* containerView;

- (id)initWithBrowser:(Browser*)browser
        containerView:(BrowserActionsContainerView*)container;

- (void)update;

- (NSUInteger)buttonCount;

- (NSUInteger)visibleButtonCount;

- (MenuButton*)chevronMenuButton;

- (void)resizeContainerAndAnimate:(BOOL)animate;

- (NSView*)browserActionViewForExtension:(const Extension*)extension;

- (CGFloat)savedWidth;

- (NSPoint)popupPointForBrowserAction:(const Extension*)extension;

- (BOOL)chevronIsHidden;

+ (void)registerUserPrefs:(PrefService*)prefs;

@end  

@interface BrowserActionsController(TestingAPI)
- (NSButton*)buttonWithIndex:(NSUInteger)index;
@end

#endif  
