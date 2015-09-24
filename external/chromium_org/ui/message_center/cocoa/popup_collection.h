// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_COCOA_POPUP_COLLECTION_H_
#define UI_MESSAGE_CENTER_COCOA_POPUP_COLLECTION_H_

#import <Cocoa/Cocoa.h>

#include <set>

#include "base/mac/scoped_block.h"
#import "base/mac/scoped_nsobject.h"
#import "base/memory/scoped_ptr.h"
#include "ui/message_center/message_center_export.h"

namespace message_center {
class MessageCenter;
class MessageCenterObserver;
}

namespace message_center {
typedef void(^AnimationEndedCallback)();
}

MESSAGE_CENTER_EXPORT
@interface MCPopupCollection : NSObject {
 @private
  
  message_center::MessageCenter* messageCenter_;

  
  scoped_ptr<message_center::MessageCenterObserver> observer_;

  
  base::scoped_nsobject<NSMutableArray> popups_;

  
  
  base::scoped_nsobject<NSMutableArray> popupsBeingRemoved_;

  
  
  NSRect testingScreenFrame_;

  
  NSTimeInterval popupAnimationDuration_;

  
  
  std::set<std::string> pendingUpdateNotificationIDs_;

  
  
  std::set<std::string> pendingRemoveNotificationIDs_;

  
  
  std::set<std::string> animatingNotificationIDs_;

  
  
  base::mac::ScopedBlock<message_center::AnimationEndedCallback>
      testingAnimationEndedCallback_;
}

- (id)initWithMessageCenter:(message_center::MessageCenter*)messageCenter;

- (BOOL)isAnimating;

- (NSTimeInterval)popupAnimationDuration;

- (void)onPopupAnimationEnded:(const std::string&)notificationID;

@end

@interface MCPopupCollection (ExposedForTesting)
- (NSArray*)popups;

- (void)setScreenFrame:(NSRect)frame;

- (void)setAnimationDuration:(NSTimeInterval)duration;

- (void)setAnimationEndedCallback:
    (message_center::AnimationEndedCallback)callback;
@end

#endif  
