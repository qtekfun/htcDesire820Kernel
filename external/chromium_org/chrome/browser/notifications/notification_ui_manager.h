// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_UI_MANAGER_H_
#define CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_UI_MANAGER_H_

#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"

class GURL;
class Notification;
class PrefService;
class Profile;

class NotificationUIManager {
 public:
  virtual ~NotificationUIManager() {}

  
  static NotificationUIManager* Create(PrefService* local_state);

  
  virtual void Add(const Notification& notification, Profile* profile) = 0;

  
  
  virtual bool Update(const Notification& notification, Profile* profile) = 0;

  
  
  virtual const Notification* FindById(
      const std::string& notification_id) const = 0;

  
  
  virtual bool CancelById(const std::string& notification_id) = 0;

  
  
  virtual std::set<std::string> GetAllIdsByProfileAndSourceOrigin(
      Profile* profile,
      const GURL& source) = 0;

  
  
  virtual bool CancelAllBySourceOrigin(const GURL& source_origin) = 0;

  
  virtual bool CancelAllByProfile(Profile* profile) = 0;

  
  
  virtual void CancelAll() = 0;

  
  
  
  
  
  static bool DelegatesToMessageCenter();

 protected:
  NotificationUIManager() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(NotificationUIManager);
};

#endif  
