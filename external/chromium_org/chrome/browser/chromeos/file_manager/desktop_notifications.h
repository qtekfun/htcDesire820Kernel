// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FILE_MANAGER_DESKTOP_NOTIFICATIONS_H_
#define CHROME_BROWSER_CHROMEOS_FILE_MANAGER_DESKTOP_NOTIFICATIONS_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "base/time/time.h"

class Profile;

namespace file_manager {

class DesktopNotifications
    : public base::SupportsWeakPtr<DesktopNotifications> {
 public:
  
  enum NotificationType {
    DEVICE,
    DEVICE_FAIL,
    DEVICE_EXTERNAL_STORAGE_DISABLED,
    FORMAT_START,
    FORMAT_START_FAIL,
    FORMAT_SUCCESS,
    FORMAT_FAIL,
  };

  explicit DesktopNotifications(Profile* profile);
  virtual ~DesktopNotifications();

  
  
  void RegisterDevice(const std::string& system_path);

  
  
  void UnregisterDevice(const std::string& system_path);

  void ManageNotificationsOnMountCompleted(const std::string& system_path,
                                           const std::string& label,
                                           bool is_parent,
                                           bool success,
                                           bool is_unsupported);

  
  void ShowNotification(NotificationType type, const std::string& path);
  void ShowNotificationDelayed(NotificationType type,
                               const std::string& path,
                               base::TimeDelta delay);

  
  virtual void HideNotification(NotificationType type, const std::string& path);
  void HideNotificationDelayed(NotificationType type,
                               const std::string& path,
                               base::TimeDelta delay);

  size_t GetNotificationCountForTest() const {
    return notification_map_.size();
  }

  bool HasNotificationForTest(const std::string& id) const {
    return notification_map_.find(id) != notification_map_.end();
  }

  base::string16 GetNotificationMessageForTest(const std::string& id) const;

 private:
  class NotificationMessage;
  struct MountRequestsInfo;

  typedef std::map<std::string, MountRequestsInfo> MountRequestsMap;
  typedef std::map<std::string, NotificationMessage*> NotificationMap;

  
  virtual void ShowNotificationWithMessage(NotificationType type,
                                           const std::string& path,
                                           const base::string16& message);
  void ShowNotificationById(NotificationType type,
                            const std::string& notification_id,
                            const base::string16& message);
  void HideNotificationById(const std::string& notification_id);
  void RemoveNotificationById(const std::string& notification_id);

  NotificationMap notification_map_;
  std::set<std::string> hidden_notifications_;
  MountRequestsMap mount_requests_;
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(DesktopNotifications);
};

}  

#endif  
