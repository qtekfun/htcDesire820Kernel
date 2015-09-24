// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_EXTENSIONS_BROWSER_ACTION_BUTTON_H_
#define CHROME_BROWSER_UI_COCOA_EXTENSIONS_BROWSER_ACTION_BUTTON_H_
#pragma once

#import <Cocoa/Cocoa.h>

#import "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/gradient_button_cell.h"

class Extension;
class ExtensionAction;
class ExtensionImageTrackerBridge;
class Profile;

extern NSString* const kBrowserActionButtonUpdatedNotification;

extern NSString* const kBrowserActionButtonDraggingNotification;
extern NSString* const kBrowserActionButtonDragEndNotification;

@interface BrowserActionButton : NSButton {
 @private
  
  
  scoped_ptr<ExtensionImageTrackerBridge> imageLoadingBridge_;

  
  scoped_nsobject<NSImage> defaultIcon_;

  
  scoped_nsobject<NSImage> tabSpecificIcon_;

  
  scoped_nsobject<NSViewAnimation> moveAnimation_;

  
  const Extension* extension_;

  
  int tabId_;

  
  BOOL isBeingDragged_;

  
  
  
  BOOL dragCouldStart_;
}

- (id)initWithFrame:(NSRect)frame
          extension:(const Extension*)extension
            profile:(Profile*)profile
              tabId:(int)tabId;

- (void)setFrame:(NSRect)frameRect animate:(BOOL)animate;

- (void)setDefaultIcon:(NSImage*)image;

- (void)setTabSpecificIcon:(NSImage*)image;

- (void)updateState;

- (BOOL)isAnimating;

- (NSImage*)compositedImage;

@property(readonly, nonatomic) BOOL isBeingDragged;
@property(readonly, nonatomic) const Extension* extension;
@property(readwrite, nonatomic) int tabId;

@end

@interface BrowserActionCell : GradientButtonCell {
 @private
  
  int tabId_;

  
  ExtensionAction* extensionAction_;
}

@property(readwrite, nonatomic) int tabId;
@property(readwrite, nonatomic) ExtensionAction* extensionAction;

@end

#endif  
