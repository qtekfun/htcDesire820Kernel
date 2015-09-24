// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_POPUP_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_POPUP_CONTROLLER_H_
#pragma once

#import <Cocoa/Cocoa.h>

#import "base/mac/cocoa_protocols.h"
#import "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/cocoa/info_bubble_view.h"
#include "googleurl/src/gurl.h"


class Browser;
class DevtoolsNotificationBridge;
class ExtensionHost;
@class InfoBubbleWindow;
class NotificationRegistrar;

@interface ExtensionPopupController : NSWindowController<NSWindowDelegate> {
 @private
  
  NSView* extensionView_;

  
  NSWindow* parentWindow_;

  
  
  NSPoint anchor_;

  
  
  NSRect extensionFrame_;

  
  scoped_ptr<ExtensionHost> host_;

  scoped_ptr<NotificationRegistrar> registrar_;
  scoped_ptr<DevtoolsNotificationBridge> notificationBridge_;

  
  BOOL beingInspected_;
}

- (ExtensionHost*)extensionHost;

+ (ExtensionPopupController*)showURL:(GURL)url
                           inBrowser:(Browser*)browser
                          anchoredAt:(NSPoint)anchoredAt
                       arrowLocation:(info_bubble::BubbleArrowLocation)
                                         arrowLocation
                             devMode:(BOOL)devMode;

+ (ExtensionPopupController*)popup;

- (BOOL)isClosing;

- (void)showDevTools;
@end

@interface ExtensionPopupController(TestingAPI)
- (NSView*)view;
+ (NSSize)minPopupSize;
+ (NSSize)maxPopupSize;
@end

#endif  
