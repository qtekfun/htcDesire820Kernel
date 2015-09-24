// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_COCOA_TRAY_VIEW_CONTROLLER_H_
#define UI_MESSAGE_CENTER_COCOA_TRAY_VIEW_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include <list>
#include <map>
#include <string>

#include "base/mac/scoped_block.h"
#import "base/mac/scoped_nsobject.h"
#include "base/strings/string16.h"
#include "ui/message_center/message_center_export.h"

@class HoverImageButton;
@class MCNotificationController;
@class MCSettingsController;

namespace message_center {
class MessageCenter;
}

@class HoverImageButton;
@class MCClipView;

namespace message_center {
typedef void(^TrayAnimationEndedCallback)();
}

MESSAGE_CENTER_EXPORT
@interface MCTrayViewController : NSViewController<NSAnimationDelegate> {
 @private
  
  message_center::MessageCenter* messageCenter_;

  
  base::scoped_nsobject<HoverImageButton> backButton_;

  
  base::scoped_nsobject<NSTextField> title_;

  
  base::scoped_nsobject<NSScrollView> scrollView_;

  
  base::scoped_nsobject<MCClipView> clipView_;

  
  base::scoped_nsobject<NSMutableArray> notifications_;

  
  
  std::map<std::string, MCNotificationController*> notificationsMap_;

  
  base::scoped_nsobject<HoverImageButton> pauseButton_;

  
  base::scoped_nsobject<HoverImageButton> clearAllButton_;

  
  base::scoped_nsobject<HoverImageButton> settingsButton_;

  
  
  base::scoped_nsobject<NSMutableArray> notificationsPendingRemoval_;

  
  
  base::scoped_nsobject<NSViewAnimation> animation_;

  
  base::scoped_nsobject<MCSettingsController> settingsController_;

  
  
  
  BOOL clearAllDelayed_;

  
  BOOL clearAllInProgress_;

  
  
  
  std::list<MCNotificationController*> visibleNotificationsPendingClear_;

  
  base::scoped_nsobject<NSMutableArray> clearAllAnimations_;

  
  NSTimeInterval animationDuration_;

  
  
  NSTimeInterval animateClearingNextNotificationDelay_;

  
  
  base::mac::ScopedBlock<message_center::TrayAnimationEndedCallback>
      testingAnimationEndedCallback_;
}

@property(copy, nonatomic) NSString* trayTitle;

- (id)initWithMessageCenter:(message_center::MessageCenter*)messageCenter;

- (void)onWindowClosing;

- (void)onMessageCenterTrayChanged;

- (void)toggleQuietMode:(id)sender;

- (void)clearAllNotifications:(id)sender;

- (void)showSettings:(id)sender;

- (void)updateSettings;

- (void)showMessages:(id)sender;

- (void)cleanupSettings;

- (void)scrollToTop;

- (BOOL)isAnimating;

+ (CGFloat)maxTrayClientHeight;

+ (CGFloat)trayWidth;

@end


@interface MCTrayViewController (TestingAPI)
- (NSScrollView*)scrollView;
- (HoverImageButton*)pauseButton;
- (HoverImageButton*)clearAllButton;

- (void)setAnimationDuration:(NSTimeInterval)duration;

- (void)setAnimateClearingNextNotificationDelay:(NSTimeInterval)delay;

- (void)setAnimationEndedCallback:
    (message_center::TrayAnimationEndedCallback)callback;
@end

#endif  
