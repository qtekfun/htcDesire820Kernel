// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_COCOA_STATUS_ITEM_VIEW_H_
#define UI_MESSAGE_CENTER_COCOA_STATUS_ITEM_VIEW_H_

#import <AppKit/AppKit.h>

#include "base/mac/scoped_block.h"
#include "base/mac/scoped_nsobject.h"
#include "ui/message_center/message_center_export.h"

namespace message_center {

typedef void(^StatusItemClickedCallack)();

}  

MESSAGE_CENTER_EXPORT
@interface MCStatusItemView : NSView {
 @private
  
  base::scoped_nsobject<NSStatusItem> statusItem_;

  
  base::mac::ScopedBlock<message_center::StatusItemClickedCallack> callback_;

  
  size_t unreadCount_;

  
  BOOL quietMode_;

  
  BOOL highlight_;

  
  
  BOOL inMouseEventSequence_;
}

@property(copy, nonatomic) message_center::StatusItemClickedCallack callback;
@property(nonatomic) BOOL highlight;

- (id)init;

- (void)setUnreadCount:(size_t)unreadCount withQuietMode:(BOOL)quietMode;

- (void)removeItem;

@end

@interface MCStatusItemView (TestingAPI)

- (size_t)unreadCount;

@end

#endif  
