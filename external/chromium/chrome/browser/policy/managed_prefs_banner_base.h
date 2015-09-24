// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_MANAGED_PREFS_BANNER_BASE_H_
#define CHROME_BROWSER_POLICY_MANAGED_PREFS_BANNER_BASE_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/options/options_window.h"
#include "content/common/notification_observer.h"

class PrefService;
class PrefSetObserver;

namespace policy {

class ManagedPrefsBannerBase : public NotificationObserver {
 public:
  
  
  
  ManagedPrefsBannerBase(PrefService* local_state,
                         PrefService* user_prefs,
                         OptionsPage page);

  
  
  ManagedPrefsBannerBase(PrefService* user_prefs, OptionsPage page);

  virtual ~ManagedPrefsBannerBase();

  
  bool DetermineVisibility() const;

  
  void AddLocalStatePref(const char* pref);
  
  void RemoveLocalStatePref(const char* pref);

  
  void AddUserPref(const char* pref);
  
  void RemoveUserPref(const char* pref);

 protected:
  
  
  
  virtual void OnUpdateVisibility() { }

 private:
  
  void Init(PrefService* local_state,
            PrefService* user_prefs,
            OptionsPage page);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  scoped_ptr<PrefSetObserver> local_state_set_;
  scoped_ptr<PrefSetObserver> user_pref_set_;

  DISALLOW_COPY_AND_ASSIGN(ManagedPrefsBannerBase);
};

}  

#endif  
