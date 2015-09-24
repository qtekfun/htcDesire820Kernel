// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_PREF_SET_OBSERVER_H_
#define CHROME_BROWSER_PREFS_PREF_SET_OBSERVER_H_
#pragma once

#include <set>

#include "base/basictypes.h"
#include "chrome/browser/prefs/pref_change_registrar.h"
#include "chrome/browser/prefs/pref_service.h"
#include "content/common/notification_observer.h"

class PrefSetObserver : public NotificationObserver {
 public:
  
  PrefSetObserver(PrefService* pref_service,
                  NotificationObserver* observer);
  virtual ~PrefSetObserver();

  
  void AddPref(const std::string& pref);
  
  void RemovePref(const std::string& pref);

  
  bool IsObserved(const std::string& pref);
  
  bool IsManaged();

  
  static PrefSetObserver* CreateProxyPrefSetObserver(
      PrefService* pref_service,
      NotificationObserver* observer);

  
  static PrefSetObserver* CreateDefaultSearchPrefSetObserver(
      PrefService* pref_service,
      NotificationObserver* observer);

 private:
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  typedef std::set<std::string> PrefSet;
  PrefSet prefs_;

  PrefService* pref_service_;
  PrefChangeRegistrar registrar_;
  NotificationObserver* observer_;

  DISALLOW_COPY_AND_ASSIGN(PrefSetObserver);
};

#endif  
