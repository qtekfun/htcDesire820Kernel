// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_NOTIFICATION_TYPES_H_
#define UI_MESSAGE_CENTER_NOTIFICATION_TYPES_H_


#include <string>

#include "ui/message_center/message_center_export.h"

namespace message_center {

MESSAGE_CENTER_EXPORT extern const char kPriorityKey[];
MESSAGE_CENTER_EXPORT extern const char kTimestampKey[];
MESSAGE_CENTER_EXPORT extern const char kButtonOneTitleKey[];
MESSAGE_CENTER_EXPORT extern const char kButtonOneIconUrlKey[];
MESSAGE_CENTER_EXPORT extern const char kButtonTwoTitleKey[];
MESSAGE_CENTER_EXPORT extern const char kButtonTwoIconUrlKey[];
MESSAGE_CENTER_EXPORT extern const char kExpandedMessageKey[];
MESSAGE_CENTER_EXPORT extern const char kImageUrlKey[];
MESSAGE_CENTER_EXPORT extern const char kItemsKey[];
MESSAGE_CENTER_EXPORT extern const char kItemTitleKey[];
MESSAGE_CENTER_EXPORT extern const char kItemMessageKey[];
MESSAGE_CENTER_EXPORT extern const char kPrivateNeverTimeoutKey[];

enum NotificationType {
  NOTIFICATION_TYPE_SIMPLE,
  NOTIFICATION_TYPE_BASE_FORMAT,
  NOTIFICATION_TYPE_IMAGE,
  NOTIFICATION_TYPE_MULTIPLE,
  NOTIFICATION_TYPE_PROGRESS
};

enum NotificationPriority {
  MIN_PRIORITY = -2,
  LOW_PRIORITY = -1,
  DEFAULT_PRIORITY = 0,
  HIGH_PRIORITY = 1,
  MAX_PRIORITY = 2,

  
  
  
  SYSTEM_PRIORITY = 3,
};

}  

#endif 
