// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_MEDIA_DEVICE_ID_SALT_H_
#define CHROME_BROWSER_MEDIA_MEDIA_DEVICE_ID_SALT_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/prefs/pref_member.h"
#include "components/user_prefs/pref_registry_syncable.h"

class PrefService;

class MediaDeviceIDSalt : public base::RefCountedThreadSafe<MediaDeviceIDSalt> {
 public:
  MediaDeviceIDSalt(PrefService* pref_service, bool incognito);
  void ShutdownOnUIThread();

  std::string GetSalt() const;

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);
  static void Reset(PrefService* pref_service);

 private:
  friend class base::RefCountedThreadSafe<MediaDeviceIDSalt>;
  ~MediaDeviceIDSalt();

  
  
  std::string incognito_salt_;
  mutable StringPrefMember media_device_id_salt_;

  DISALLOW_COPY_AND_ASSIGN(MediaDeviceIDSalt);
};

#endif  
