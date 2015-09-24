// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_DELEGATE_H_
#define CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_DELEGATE_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "ui/message_center/notification_delegate.h"

namespace content {
class RenderViewHost;
}

class NotificationDelegate : public message_center::NotificationDelegate {
 public:
  
  virtual std::string id() const = 0;

  
  virtual int process_id() const;

  
  virtual content::RenderViewHost* GetRenderViewHost() const = 0;

  
  virtual void ReleaseRenderViewHost();

 protected:
  virtual ~NotificationDelegate() {}
};

#endif  
