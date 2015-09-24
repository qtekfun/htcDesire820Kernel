// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/chrome_event_processing_window.h"

class AppNotificationBridge;

namespace info_bubble {

enum AnimationMask {
  kAnimateNone = 0,
  kAnimateOrderIn = 1 << 1,
  kAnimateOrderOut = 1 << 2,
};
typedef NSUInteger AllowedAnimations;

}  

@interface InfoBubbleWindow : ChromeEventProcessingWindow {
 @private
  
  BOOL closing_;
  
  
  info_bubble::AllowedAnimations allowedAnimations_;
  
  
  BOOL canBecomeKeyWindow_;
  
  scoped_ptr<AppNotificationBridge> notificationBridge_;
}

@property(nonatomic) info_bubble::AllowedAnimations allowedAnimations;
@property(nonatomic) BOOL canBecomeKeyWindow;

- (BOOL)isClosing;

@end
