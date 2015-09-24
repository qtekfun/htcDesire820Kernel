// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_RECOMMENDATION_RESTORER_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_RECOMMENDATION_RESTORER_H_

#include <string>

#include "ash/wm/user_activity_observer.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/timer/timer.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Profile;

namespace policy {

class RecommendationRestorer : public BrowserContextKeyedService,
                               public content::NotificationObserver,
                               public ash::UserActivityObserver {
 public:
  explicit RecommendationRestorer(Profile* profile);
  virtual ~RecommendationRestorer();

  
  virtual void Shutdown() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnUserActivity(const ui::Event* event) OVERRIDE;

  
  
  
  
  
  void Restore(bool allow_delay, const std::string& pref_name);

 private:
  friend class RecommendationRestorerTest;

  void RestoreAll();

  void StartTimer();
  void StopTimer();

  PrefChangeRegistrar pref_change_registrar_;
  content::NotificationRegistrar notification_registrar_;

  bool logged_in_;

  base::OneShotTimer<RecommendationRestorer> restore_timer_;

  DISALLOW_COPY_AND_ASSIGN(RecommendationRestorer);
};

}  

#endif  
