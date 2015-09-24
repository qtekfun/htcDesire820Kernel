// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_COCOA_POPUP_CONTROLLER_H_
#define UI_MESSAGE_CENTER_COCOA_POPUP_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include <string>

#import "base/mac/scoped_nsobject.h"
#import "ui/base/cocoa/tracking_area.h"
#include "ui/message_center/message_center_export.h"

namespace message_center {
class MessageCenter;
class Notification;
}

@class MCNotificationController;
@class MCPopupCollection;

MESSAGE_CENTER_EXPORT
@interface MCPopupController : NSWindowController<NSAnimationDelegate>  {
 @private
  
  message_center::MessageCenter* messageCenter_;

  
  MCPopupCollection* popupCollection_;

  
  base::scoped_nsobject<MCNotificationController> notificationController_;

  
  BOOL swipeGestureEnded_;

  
  
  NSRect originalFrame_;

  
  BOOL isClosing_;

  
  
  NSRect bounds_;

  
  base::scoped_nsobject<NSViewAnimation> boundsAnimation_;

  
  ui::ScopedCrTrackingArea trackingArea_;
}

- (id)initWithNotification:(const message_center::Notification*)notification
             messageCenter:(message_center::MessageCenter*)messageCenter
           popupCollection:(MCPopupCollection*)popupCollection;

- (MCNotificationController*)notificationController;

- (const message_center::Notification*)notification;

- (const std::string&)notificationID;

- (void)showWithAnimation:(NSRect)newBounds;

- (void)closeWithAnimation;

- (void)markPopupCollectionGone;

- (NSRect)bounds;

- (void)setBounds:(NSRect)newBounds;

@end

#endif  
