// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_INSTALLED_BUBBLE_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_INSTALLED_BUBBLE_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/browser_window_controller.h"
#import "chrome/browser/ui/cocoa/base_bubble_controller.h"
#include "third_party/skia/include/core/SkBitmap.h"

class Browser;
class ExtensionLoadedNotificationObserver;
@class HyperlinkTextView;
@class HoverCloseButton;
@class InfoBubbleView;

namespace extensions {
class BundleInstaller;
class Extension;
}

namespace extension_installed_bubble {

const int kIconSize = 43;

const int kOuterVerticalMargin = 15;

const int kInnerVerticalMargin = 10;

const int kAppsBubbleArrowOffset = 4;

typedef enum {
  kApp,
  kBrowserAction,
  kGeneric,
  kOmniboxKeyword,
  kPageAction,
  kBundle,
} ExtensionType;

}  

@interface ExtensionInstalledBubbleController :
    BaseBubbleController<NSTextViewDelegate> {
 @private
  const extensions::Extension* extension_;  
  const extensions::BundleInstaller* bundle_;  
  Browser* browser_;  
  base::scoped_nsobject<NSImage> icon_;

  extension_installed_bubble::ExtensionType type_;

  
  
  
  BOOL pageActionPreviewShowing_;

  
  
  
  scoped_ptr<ExtensionLoadedNotificationObserver> extensionObserver_;

  
  IBOutlet HoverCloseButton* closeButton_;
  IBOutlet NSImageView* iconImage_;
  IBOutlet NSTextField* heading_;
  
  IBOutlet NSTextField* howToUse_;
  IBOutlet NSTextField* howToManage_;
  
  IBOutlet NSButton* appShortcutLink_;
  
  IBOutlet NSButton* manageShortcutLink_;
  
  IBOutlet NSTextField* promoPlaceholder_;
  
  
  
  
  base::scoped_nsobject<HyperlinkTextView> promo_;
  
  IBOutlet NSTextField* installedHeadingMsg_;
  IBOutlet NSTextField* installedItemsMsg_;
  IBOutlet NSTextField* failedHeadingMsg_;
  IBOutlet NSTextField* failedItemsMsg_;
}

@property(nonatomic, readonly) const extensions::Extension* extension;
@property(nonatomic, readonly) const extensions::BundleInstaller* bundle;
@property(nonatomic) BOOL pageActionPreviewShowing;

- (id)initWithParentWindow:(NSWindow*)parentWindow
                 extension:(const extensions::Extension*)extension
                    bundle:(const extensions::BundleInstaller*)bundle
                   browser:(Browser*)browser
                      icon:(SkBitmap)icon;

- (IBAction)closeWindow:(id)sender;

- (BOOL)textView:(NSTextView*)aTextView
   clickedOnLink:(id)link
         atIndex:(NSUInteger)charIndex;

- (void)showWindow:(id)sender;

- (void)extensionUnloaded:(id)sender;

- (IBAction)onManageShortcutClicked:(id)sender;

- (IBAction)onAppShortcutClicked:(id)sender;

@end

@interface ExtensionInstalledBubbleController (ExposedForTesting)

- (void)removePageActionPreviewIfNecessary;
- (NSWindow*)initializeWindow;
- (int)calculateWindowHeight;
- (void)setMessageFrames:(int)newWindowHeight;
- (NSRect)headingFrame;
- (NSRect)frameOfHowToUse;
- (NSRect)frameOfHowToManage;
- (NSRect)frameOfSigninPromo;
- (BOOL)showSyncPromo;
- (NSButton*)appInstalledShortcutLink;

@end  

#endif  
