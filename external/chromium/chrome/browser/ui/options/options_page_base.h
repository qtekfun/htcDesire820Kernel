// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_OPTIONS_OPTIONS_PAGE_BASE_H_
#define CHROME_BROWSER_UI_OPTIONS_OPTIONS_PAGE_BASE_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "chrome/browser/metrics/user_metrics.h"
#include "chrome/browser/ui/options/options_window.h"
#include "content/common/notification_observer.h"

class PrefService;
class Profile;
struct UserMetricsAction;

class OptionsPageBase : public NotificationObserver {
 public:
  virtual ~OptionsPageBase();

  
  virtual void HighlightGroup(OptionsGroup highlight_group) { }

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 protected:
  
  
  explicit OptionsPageBase(Profile* profile);

  
  Profile* profile() const { return profile_; }

  
  void UserMetricsRecordAction(const UserMetricsAction &action,
                               PrefService* prefs);

  
  
  
  
  virtual void NotifyPrefChanged(const std::string* pref_name) {}

 private:
  
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(OptionsPageBase);
};

#endif  
