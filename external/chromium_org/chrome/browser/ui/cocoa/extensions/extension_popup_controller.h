// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_POPUP_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_POPUP_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/base_bubble_controller.h"
#import "chrome/browser/ui/cocoa/info_bubble_view.h"
#include "url/gurl.h"


class Browser;
class DevtoolsNotificationBridge;
class ExtensionPopupContainer;

namespace content {
class NotificationRegistrar;
}

namespace extensions {
class ExtensionViewHost;
}

@interface ExtensionPopupController : BaseBubbleController {
 @private
  
  NSView* extensionView_;

  
  
  NSRect extensionFrame_;

  
  scoped_ptr<extensions::ExtensionViewHost> host_;

  scoped_ptr<content::NotificationRegistrar> registrar_;
  scoped_ptr<DevtoolsNotificationBridge> notificationBridge_;
  scoped_ptr<ExtensionPopupContainer> container_;

  
  BOOL beingInspected_;

  
  
  BOOL ignoreWindowDidResignKey_;

  
  NSSize pendingSize_;
}

- (extensions::ExtensionViewHost*)extensionViewHost;

+ (ExtensionPopupController*)showURL:(GURL)url
                           inBrowser:(Browser*)browser
                          anchoredAt:(NSPoint)anchoredAt
                       arrowLocation:(info_bubble::BubbleArrowLocation)
                                         arrowLocation
                             devMode:(BOOL)devMode;

+ (ExtensionPopupController*)popup;

- (BOOL)isClosing;

- (void)showDevTools;

- (void)setBeingInspected:(BOOL)beingInspected;
@end

@interface ExtensionPopupController(TestingAPI)
- (NSView*)view;
+ (NSSize)minPopupSize;
+ (NSSize)maxPopupSize;
@end

#endif  
