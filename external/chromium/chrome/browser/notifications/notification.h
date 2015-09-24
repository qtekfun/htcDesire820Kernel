// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_H_
#define CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "chrome/browser/notifications/notification_object_proxy.h"
#include "googleurl/src/gurl.h"

class NotificationDelegate;

class Notification {
 public:
  Notification(const GURL& origin_url,
               const GURL& content_url,
               const string16& display_source,
               const string16& replace_id,
               NotificationDelegate* delegate);
  Notification(const Notification& notification);
  ~Notification();
  Notification& operator=(const Notification& notification);

  
  const GURL& content_url() const { return content_url_; }

  
  const GURL& origin_url() const { return origin_url_; }

  
  const string16& display_source() const { return display_source_; }

  const string16& replace_id() const { return replace_id_; }

  void Display() const { delegate()->Display(); }
  void Error() const { delegate()->Error(); }
  void Click() const { delegate()->Click(); }
  void Close(bool by_user) const { delegate()->Close(by_user); }

  std::string notification_id() const { return delegate()->id(); }

 private:
  NotificationDelegate* delegate() const { return delegate_.get(); }

  
  GURL origin_url_;

  
  
  GURL content_url_;

  
  
  string16 display_source_;

  
  string16 replace_id_;

  
  
  scoped_refptr<NotificationDelegate> delegate_;
};

#endif  
