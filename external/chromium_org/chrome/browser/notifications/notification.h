// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_H_
#define CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "base/values.h"
#include "chrome/browser/notifications/notification_delegate.h"
#include "third_party/WebKit/public/web/WebTextDirection.h"
#include "ui/gfx/image/image.h"
#include "ui/message_center/notification.h"
#include "ui/message_center/notification_types.h"
#include "url/gurl.h"

class Notification : public message_center::Notification {
 public:
  
  Notification(const GURL& origin_url,
               const GURL& content_url,
               const base::string16& display_source,
               const base::string16& replace_id,
               NotificationDelegate* delegate);

  
  
  Notification(const GURL& origin_url,
               const GURL& icon_url,
               const base::string16& title,
               const base::string16& body,
               blink::WebTextDirection dir,
               const base::string16& display_source,
               const base::string16& replace_id,
               NotificationDelegate* delegate);

  
  
  Notification(const GURL& origin_url,
               const gfx::Image& icon,
               const base::string16& title,
               const base::string16& body,
               blink::WebTextDirection dir,
               const base::string16& display_source,
               const base::string16& replace_id,
               NotificationDelegate* delegate);

  Notification(
      message_center::NotificationType type,
      const GURL& origin_url,
      const base::string16& title,
      const base::string16& body,
      const gfx::Image& icon,
      blink::WebTextDirection dir,
      const message_center::NotifierId& notifier_id,
      const base::string16& display_source,
      const base::string16& replace_id,
      const message_center::RichNotificationData& rich_notification_data,
      NotificationDelegate* delegate);

  Notification(const Notification& notification);
  virtual ~Notification();
  Notification& operator=(const Notification& notification);

  
  bool is_html() const { return is_html_; }

  
  const GURL& content_url() const { return content_url_; }

  
  const GURL& origin_url() const { return origin_url_; }

  
  const GURL& icon_url() const { return icon_url_; }

  
  const base::string16& replace_id() const { return replace_id_; }

  
  const GURL& button_one_icon_url() const { return button_one_icon_url_; }
  const GURL& button_two_icon_url() const { return button_two_icon_url_; }

  
  const GURL& image_url() const { return image_url_; }

  std::string notification_id() const { return delegate()->id(); }
  int process_id() const { return delegate()->process_id(); }

  content::RenderViewHost* GetRenderViewHost() const {
    return delegate()->GetRenderViewHost();
  }
  void DoneRendering() { delegate()->ReleaseRenderViewHost(); }

  NotificationDelegate* delegate() const { return delegate_.get(); }

 private:
  
  GURL origin_url_;

  
  
  GURL icon_url_;

  
  
  bool is_html_;

  
  
  GURL content_url_;

  
  GURL button_one_icon_url_;
  GURL button_two_icon_url_;

  
  GURL image_url_;

  
  base::string16 replace_id_;

  
  
  scoped_refptr<NotificationDelegate> delegate_;
};

#endif  
