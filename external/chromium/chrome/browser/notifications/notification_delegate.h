// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_DELEGATE_H_
#define CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_DELEGATE_H_
#pragma once

#include <string>

#include "base/memory/ref_counted.h"

class NotificationDelegate
    : public base::RefCountedThreadSafe<NotificationDelegate> {
 public:
  
  virtual void Display() = 0;

  
  
  virtual void Error() = 0;

  
  
  virtual void Close(bool by_user) = 0;

  
  virtual void Click() = 0;

  
  virtual std::string id() const = 0;

 protected:
  virtual ~NotificationDelegate() {}

 private:
  friend class base::RefCountedThreadSafe<NotificationDelegate>;
};

#endif  
