// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/chrome_event_processing_window.h"

class AppNotificationBridge;

@interface InfoBubbleWindow : ChromeEventProcessingWindow {
 @private
  
  BOOL closing_;
  
  
  BOOL delayOnClose_;
  
  scoped_ptr<AppNotificationBridge> notificationBridge_;
}

@property(nonatomic) BOOL delayOnClose;

- (BOOL)isClosing;

@end
