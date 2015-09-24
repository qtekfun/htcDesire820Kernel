// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_OBJECT_PROXY_H_
#define CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_OBJECT_PROXY_H_

#include <string>

#include "chrome/browser/notifications/notification_delegate.h"

class NotificationObjectProxy
    : public NotificationDelegate {
 public:
  
  NotificationObjectProxy(int process_id, int route_id,
                          int notification_id, bool worker);

  
  virtual void Display() OVERRIDE;
  virtual void Error() OVERRIDE;
  virtual void Close(bool by_user) OVERRIDE;
  virtual void Click() OVERRIDE;
  virtual std::string id() const OVERRIDE;
  virtual int process_id() const OVERRIDE;
  virtual content::RenderViewHost* GetRenderViewHost() const OVERRIDE;

 protected:
  friend class base::RefCountedThreadSafe<NotificationObjectProxy>;

  virtual ~NotificationObjectProxy() {}

 private:
  
  int process_id_;
  int route_id_;
  int notification_id_;
  bool worker_;
  bool displayed_;
};

#endif  
