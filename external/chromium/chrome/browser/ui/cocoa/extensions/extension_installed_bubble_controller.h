// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_INSTALLED_BUBBLE_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_INSTALLED_BUBBLE_CONTROLLER_H_
#pragma once

#import <Cocoa/Cocoa.h>

#import "base/mac/cocoa_protocols.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/browser_window_controller.h"
#include "third_party/skia/include/core/SkBitmap.h"

class Browser;
class Extension;
class ExtensionLoadedNotificationObserver;
@class HoverCloseButton;
@class InfoBubbleView;

namespace extension_installed_bubble {

const int kIconSize = 43;

const int kOuterVerticalMargin = 15;

const int kInnerVerticalMargin = 10;

typedef enum {
  kBrowserAction,
  kGeneric,
  kOmniboxKeyword,
  kPageAction
} ExtensionType;

}

@interface ExtensionInstalledBubbleController :
    NSWindowController<NSWindowDelegate> {
 @private
  NSWindow* parentWindow_;  
  const Extension* extension_;  
  Browser* browser_;  
  scoped_nsobject<NSImage> icon_;

  extension_installed_bubble::ExtensionType type_;

  
  
  
  BOOL pageActionRemoved_;

  
  
  
  scoped_ptr<ExtensionLoadedNotificationObserver> extensionObserver_;

  
  IBOutlet InfoBubbleView* infoBubbleView_;
  IBOutlet HoverCloseButton* closeButton_;
  IBOutlet NSImageView* iconImage_;
  IBOutlet NSTextField* extensionInstalledMsg_;
  
  IBOutlet NSTextField* extraInfoMsg_;
  IBOutlet NSTextField* extensionInstalledInfoMsg_;
}

@property(nonatomic, readonly) const Extension* extension;
@property(nonatomic) BOOL pageActionRemoved;

- (id)initWithParentWindow:(NSWindow*)parentWindow
                 extension:(const Extension*)extension
                   browser:(Browser*)browser
                      icon:(SkBitmap)icon;

- (IBAction)closeWindow:(id)sender;

- (void)showWindow:(id)sender;

- (void)extensionUnloaded:(id)sender;

@end

@interface ExtensionInstalledBubbleController(ExposedForTesting)

- (void)removePageActionPreviewIfNecessary;
- (NSWindow*)initializeWindow;
- (int)calculateWindowHeight;
- (void)setMessageFrames:(int)newWindowHeight;
- (NSRect)getExtensionInstalledMsgFrame;
- (NSRect)getExtraInfoMsgFrame;
- (NSRect)getExtensionInstalledInfoMsgFrame;

@end  

#endif  
