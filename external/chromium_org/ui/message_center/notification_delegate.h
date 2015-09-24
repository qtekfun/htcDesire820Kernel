// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_NOTIFICATION_DELEGATE_H_
#define UI_MESSAGE_CENTER_NOTIFICATION_DELEGATE_H_

#include <string>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "ui/message_center/message_center_export.h"

namespace content {
class RenderViewHost;
}

namespace message_center {

class MESSAGE_CENTER_EXPORT NotificationDelegate
    : public base::RefCountedThreadSafe<NotificationDelegate> {
 public:
  
  virtual void Display() = 0;

  
  
  virtual void Error() = 0;

  
  
  virtual void Close(bool by_user) = 0;

  
  virtual bool HasClickedListener();

  
  virtual void Click() = 0;

  
  
  
  virtual void ButtonClick(int button_index);

 protected:
  virtual ~NotificationDelegate() {}

 private:
  friend class base::RefCountedThreadSafe<NotificationDelegate>;
};

class MESSAGE_CENTER_EXPORT HandleNotificationClickedDelegate
    : public NotificationDelegate {
 public:
  explicit HandleNotificationClickedDelegate(const base::Closure& closure);

  
  virtual void Display() OVERRIDE;
  virtual void Error() OVERRIDE;
  virtual void Close(bool by_user) OVERRIDE;
  virtual bool HasClickedListener() OVERRIDE;
  virtual void Click() OVERRIDE;
  virtual void ButtonClick(int button_index) OVERRIDE;

 protected:
  virtual ~HandleNotificationClickedDelegate();

 private:
  std::string id_;
  base::Closure closure_;

  DISALLOW_COPY_AND_ASSIGN(HandleNotificationClickedDelegate);
};

}  

#endif  
