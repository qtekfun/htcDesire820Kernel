// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_COCOA_NOTIFICATION_CONTROLLER_H_
#define UI_MESSAGE_CENTER_COCOA_NOTIFICATION_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include <string>

#import "base/mac/scoped_nsobject.h"
#include "ui/message_center/message_center_export.h"

namespace message_center {
class MessageCenter;
class Notification;
}

@class HoverImageButton;

MESSAGE_CENTER_EXPORT
@interface MCNotificationController : NSViewController {
 @protected
  
  const message_center::Notification* notification_;

  
  std::string notificationID_;

  
  message_center::MessageCenter* messageCenter_;

  
  base::scoped_nsobject<HoverImageButton> closeButton_;

  
  base::scoped_nsobject<NSImageView> icon_;

  
  base::scoped_nsobject<NSTextView> title_;

  
  base::scoped_nsobject<NSTextView> message_;

  
  base::scoped_nsobject<NSTextView> contextMessage_;

  
  base::scoped_nsobject<NSView> listView_;

  
  base::scoped_nsobject<NSProgressIndicator> progressBarView_;

  
  base::scoped_nsobject<NSView> bottomView_;
}

- (id)initWithNotification:(const message_center::Notification*)notification
    messageCenter:(message_center::MessageCenter*)messageCenter;

- (NSRect)updateNotification:(const message_center::Notification*)notification;

- (void)close:(id)sender;

- (const message_center::Notification*)notification;

- (const std::string&)notificationID;

- (void)notificationClicked;

@end

@interface MCNotificationController (TestingInterface)
- (NSImageView*)iconView;
@end

#endif  
