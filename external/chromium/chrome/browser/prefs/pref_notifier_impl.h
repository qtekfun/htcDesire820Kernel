// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_PREF_NOTIFIER_IMPL_H_
#define CHROME_BROWSER_PREFS_PREF_NOTIFIER_IMPL_H_
#pragma once

#include <string>

#include "base/hash_tables.h"
#include "base/observer_list.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/prefs/pref_notifier.h"

class PrefService;
class NotificationObserver;

class PrefNotifierImpl : public PrefNotifier,
                         public base::NonThreadSafe {
 public:
  explicit PrefNotifierImpl(PrefService* pref_service);
  virtual ~PrefNotifierImpl();

  
  
  void AddPrefObserver(const char* path, NotificationObserver* obs);
  void RemovePrefObserver(const char* path, NotificationObserver* obs);

  
  virtual void OnPreferenceChanged(const std::string& pref_name);
  virtual void OnInitializationCompleted();

 protected:
  
  
  
  typedef ObserverList<NotificationObserver> NotificationObserverList;
  typedef base::hash_map<std::string, NotificationObserverList*>
      PrefObserverMap;

  const PrefObserverMap* pref_observers() const { return &pref_observers_; }

 private:
  
  
  virtual void FireObservers(const std::string& path);

  
  PrefService* pref_service_;

  PrefObserverMap pref_observers_;

  DISALLOW_COPY_AND_ASSIGN(PrefNotifierImpl);
};

#endif  
