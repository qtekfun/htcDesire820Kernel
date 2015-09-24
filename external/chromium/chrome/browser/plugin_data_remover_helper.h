// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PLUGIN_DATA_REMOVER_HELPER_H_
#define CHROME_BROWSER_PLUGIN_DATA_REMOVER_HELPER_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "chrome/browser/prefs/pref_member.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class Profile;

class PluginDataRemoverHelper : public NotificationObserver {
 public:
  PluginDataRemoverHelper();
  ~PluginDataRemoverHelper();

  
  
  
  
  void Init(const char* pref_name,
            PrefService* prefs,
            NotificationObserver* observer);

  bool GetValue() const { return pref_.GetValue(); }

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  class Internal;

  BooleanPrefMember pref_;
  NotificationRegistrar registrar_;
  scoped_refptr<Internal> internal_;

  DISALLOW_COPY_AND_ASSIGN(PluginDataRemoverHelper);
};

#endif  
