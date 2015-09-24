// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_SYNC_NOTIFIER_CHROME_NOTIFIER_DELEGATE_H_
#define CHROME_BROWSER_NOTIFICATIONS_SYNC_NOTIFIER_CHROME_NOTIFIER_DELEGATE_H_

#include <map>
#include <string>

#include "chrome/browser/notifications/notification_delegate.h"
#include "url/gurl.h"

namespace notifier {

enum SyncedNotificationActionType {
  SYNCED_NOTIFICATION_ACTION_UNKNOWN,
  SYNCED_NOTIFICATION_ACTION_CLICK,
  SYNCED_NOTIFICATION_ACTION_BUTTON_CLICK,
  SYNCED_NOTIFICATION_ACTION_CLOSE_BY_USER,
  SYNCED_NOTIFICATION_ACTION_CLOSE_BY_SYSTEM,
  SYNCED_NOTIFICATION_ACTION_TOAST_TIMEOUT,
  
  
  
  SYNCED_NOTIFICATION_ACTION_COUNT
};

class ChromeNotifierService;


class ChromeNotifierDelegate : public NotificationDelegate {
 public:
  
  
  explicit ChromeNotifierDelegate(const std::string& notification_id,
                                  ChromeNotifierService* notifier);

  
  virtual void Display() OVERRIDE {}
  virtual void Error() OVERRIDE {}
  virtual void Close(bool by_user) OVERRIDE;
  virtual void Click() OVERRIDE;
  virtual void ButtonClick(int button_index) OVERRIDE;
  virtual std::string id() const OVERRIDE;

  virtual content::RenderViewHost* GetRenderViewHost() const OVERRIDE;

  void CollectAction(SyncedNotificationActionType type);

 private:
  virtual ~ChromeNotifierDelegate();
  void NavigateToUrl(const GURL& destination) const;

  const std::string notification_id_;
  ChromeNotifierService* const chrome_notifier_;

  DISALLOW_COPY_AND_ASSIGN(ChromeNotifierDelegate);
};

}  

#endif  
