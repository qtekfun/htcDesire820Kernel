// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_EXTENSIONS_BROWSER_ACTION_BUTTON_H_
#define CHROME_BROWSER_UI_COCOA_EXTENSIONS_BROWSER_ACTION_BUTTON_H_

#import <Cocoa/Cocoa.h>

#import "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/image_button_cell.h"

class Browser;
class ExtensionAction;
@class ExtensionActionContextMenuController;
class ExtensionActionIconFactoryBridge;

namespace extensions {
class Extension;
}

extern NSString* const kBrowserActionButtonDraggingNotification;
extern NSString* const kBrowserActionButtonDragEndNotification;

@interface BrowserActionButton : NSButton<NSMenuDelegate> {
 @private
  
  
  scoped_ptr<ExtensionActionIconFactoryBridge> iconFactoryBridge_;

  
  base::scoped_nsobject<NSViewAnimation> moveAnimation_;

  
  const extensions::Extension* extension_;

  
  int tabId_;

  
  BOOL isBeingDragged_;

  
  
  
  BOOL dragCouldStart_;

  
  
  NSPoint dragStartPoint_;

  base::scoped_nsobject<
      ExtensionActionContextMenuController> contextMenuController_;
}

- (id)initWithFrame:(NSRect)frame
          extension:(const extensions::Extension*)extension
            browser:(Browser*)browser
              tabId:(int)tabId;

- (void)setFrame:(NSRect)frameRect animate:(BOOL)animate;

- (void)updateState;

- (BOOL)isAnimating;

- (NSImage*)compositedImage;

@property(readonly, nonatomic) BOOL isBeingDragged;
@property(readonly, nonatomic) const extensions::Extension* extension;
@property(readwrite, nonatomic) int tabId;

@end

@interface BrowserActionCell : ImageButtonCell {
 @private
  
  int tabId_;

  
  ExtensionAction* extensionAction_;
}

@property(readwrite, nonatomic) int tabId;
@property(readwrite, nonatomic) ExtensionAction* extensionAction;

@end

#endif  
