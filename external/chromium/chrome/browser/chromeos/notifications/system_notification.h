// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_NOTIFICATIONS_SYSTEM_NOTIFICATION_H_
#define CHROME_BROWSER_CHROMEOS_NOTIFICATIONS_SYSTEM_NOTIFICATION_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/string16.h"
#include "chrome/browser/chromeos/notifications/balloon_collection_impl.h"
#include "chrome/browser/notifications/notification_delegate.h"
#include "googleurl/src/gurl.h"

class MessageCallback;
class Profile;

namespace chromeos {


class SystemNotification {
 public:
  
  
  
  SystemNotification(Profile* profile,
                     const std::string& id,
                     int icon_resource_id,
                     const string16& title);

  
  SystemNotification(Profile* profile,
                     NotificationDelegate* delegate,
                     int icon_resource_id,
                     const string16& title);

  virtual ~SystemNotification();

  void set_title(const string16& title) { title_ = title; }

  
  
  
  void Show(const string16& message, bool urgent, bool sticky);

  
  
  void Show(const string16& message, const string16& link_text,
            MessageCallback* callback, bool urgent, bool sticky);

  
  
  void Hide();

  
  bool visible() const { return visible_; }

  
  bool urgent() const { return urgent_; }

 private:
  class Delegate : public NotificationDelegate {
   public:
    explicit Delegate(const std::string& id) : id_(id) {}
    void Display() {}
    void Error() {}
    void Close(bool by_user) {}
    void Click() {}
    std::string id() const { return id_; }

   private:
    std::string id_;

    DISALLOW_COPY_AND_ASSIGN(Delegate);
  };

  void Init(int icon_resource_id);

  Profile* profile_;
  BalloonCollectionImpl* collection_;
  scoped_refptr<NotificationDelegate> delegate_;
  GURL icon_;
  string16 title_;
  bool visible_;
  bool urgent_;

  DISALLOW_COPY_AND_ASSIGN(SystemNotification);
};

}  

#endif  
