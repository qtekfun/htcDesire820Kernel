// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_ACTIVE_NOTIFICATION_TRACKER_H_
#define CONTENT_RENDERER_ACTIVE_NOTIFICATION_TRACKER_H_

#include <map>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/id_map.h"
#include "content/common/content_export.h"
#include "third_party/WebKit/public/web/WebNotification.h"

namespace blink {
class WebNotificationPermissionCallback;
}

namespace content {

class CONTENT_EXPORT ActiveNotificationTracker {
 public:
  ActiveNotificationTracker();
  ~ActiveNotificationTracker();

  
  int RegisterNotification(const blink::WebNotification& notification);
  void UnregisterNotification(int id);
  bool GetId(const blink::WebNotification& notification, int& id);
  bool GetNotification(int id, blink::WebNotification* notification);

  
  int RegisterPermissionRequest(
      blink::WebNotificationPermissionCallback* callback);
  void OnPermissionRequestComplete(int id);
  blink::WebNotificationPermissionCallback* GetCallback(int id);

  
  void Clear();

 private:
  typedef std::map<blink::WebNotification, int> ReverseTable;

  
  IDMap<blink::WebNotification> notification_table_;
  ReverseTable reverse_notification_table_;
  IDMap<blink::WebNotificationPermissionCallback> callback_table_;

  DISALLOW_COPY_AND_ASSIGN(ActiveNotificationTracker);
};

}  

#endif  
