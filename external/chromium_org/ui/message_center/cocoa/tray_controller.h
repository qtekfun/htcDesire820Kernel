// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_COCOA_TRAY_CONTROLLER_H_
#define UI_MESSAGE_CENTER_COCOA_TRAY_CONTROLLER_H_

#import <AppKit/AppKit.h>

#include "base/basictypes.h"
#include "base/mac/scoped_nsobject.h"
#include "ui/message_center/message_center_export.h"

@class MCTrayViewController;

namespace message_center {
class MessageCenterTray;
}

MESSAGE_CENTER_EXPORT
@interface MCTrayController : NSWindowController<NSWindowDelegate> {
 @private
  message_center::MessageCenterTray* tray_;  

  
  base::scoped_nsobject<MCTrayViewController> viewController_;

  
  
  id clickEventMonitor_;
}

@property(readonly, nonatomic) MCTrayViewController* viewController;

- (id)initWithMessageCenterTray:(message_center::MessageCenterTray*)tray;

- (void)showTrayAtRightOf:(NSPoint)rightPoint atLeftOf:(NSPoint)leftPoint;

- (void)onMessageCenterTrayChanged;

@end

#endif  
