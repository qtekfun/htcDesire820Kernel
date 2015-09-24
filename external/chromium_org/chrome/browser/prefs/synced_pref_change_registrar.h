// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREFS_SYNCED_PREF_CHANGE_REGISTRAR_H_
#define CHROME_BROWSER_PREFS_SYNCED_PREF_CHANGE_REGISTRAR_H_

#include <map>
#include <string>

#include "base/callback.h"
#include "chrome/browser/prefs/pref_service_syncable.h"
#include "chrome/browser/prefs/synced_pref_observer.h"

class SyncedPrefChangeRegistrar : public SyncedPrefObserver {
 public:
  
  
  
  typedef base::Callback<void(bool)> ChangeCallback;
  typedef base::Callback<void(const std::string&, bool)> NamedChangeCallback;

  explicit SyncedPrefChangeRegistrar(PrefServiceSyncable* pref_service);
  virtual ~SyncedPrefChangeRegistrar();

  
  
  void Add(const char* path, const ChangeCallback& callback);
  void Add(const char* path, const NamedChangeCallback& callback);

  
  void Remove(const char* path);

  
  void RemoveAll();

  
  bool IsObserved(const char* path) const;

 private:
  
  virtual void OnSyncedPrefChanged(const std::string& path, bool from_sync)
      OVERRIDE;

  typedef std::map<std::string, NamedChangeCallback> ObserverMap;

  PrefServiceSyncable* pref_service_;
  ObserverMap observers_;

  DISALLOW_COPY_AND_ASSIGN(SyncedPrefChangeRegistrar);
};

#endif  
