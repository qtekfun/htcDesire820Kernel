// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_NOTIFICATION_OBSERVER_H_
#define CONTENT_PUBLIC_BROWSER_NOTIFICATION_OBSERVER_H_

#include "content/common/content_export.h"

namespace content {

class NotificationDetails;
class NotificationSource;

class CONTENT_EXPORT NotificationObserver {
 public:
  virtual void Observe(int type,
                       const NotificationSource& source,
                       const NotificationDetails& details) = 0;

 protected:
  virtual ~NotificationObserver() {}
};

}  

#endif  
