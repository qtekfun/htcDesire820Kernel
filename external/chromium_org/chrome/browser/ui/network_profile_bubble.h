// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_NETWORK_PROFILE_BUBBLE_H_
#define CHROME_BROWSER_UI_NETWORK_PROFILE_BUBBLE_H_

#include "base/basictypes.h"

class Browser;
class Profile;

namespace base {
class FilePath;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class NetworkProfileBubble {
 public:
  enum MetricNetworkedProfileCheck {
   
   METRIC_CHECK_SUPPRESSED,
   
   METRIC_CHECK_FAILED,
   
   METRIC_CHECK_IO_FAILED,

   
   METRIC_PROFILE_ON_NETWORK,
   
   METRIC_PROFILE_NOT_ON_NETWORK,

   
   METRIC_REMOTE_SESSION,

   
   METRIC_LEARN_MORE_CLICKED,
   
   METRIC_ACKNOWLEDGED,

   METRIC_NETWORKED_PROFILE_CHECK_SIZE  
  };

  
  
  
  static bool ShouldCheckNetworkProfile(Profile* profile);

  
  
  static void CheckNetworkProfile(const base::FilePath& profile_folder);

  
  static void ShowNotification(Browser* browser);

  static void SetNotificationShown(bool shown);

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  static void RecordUmaEvent(MetricNetworkedProfileCheck event);

 private:
  
  
  static void NotifyNetworkProfileDetected();

  
  
  
  static bool notification_shown_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(NetworkProfileBubble);
};

#endif  
