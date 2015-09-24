// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_NET_PREF_OBSERVER_H_
#define CHROME_BROWSER_NET_NET_PREF_OBSERVER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "chrome/browser/prefs/pref_member.h"
#include "content/common/notification_observer.h"

class Profile;

namespace prerender {
class PrerenderManager;
}

class NetPrefObserver : public NotificationObserver {
 public:
  
  
  NetPrefObserver(PrefService* prefs,
                  prerender::PrerenderManager* prerender_manager);
  ~NetPrefObserver();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  static void RegisterPrefs(PrefService* prefs);

 private:
  
  void ApplySettings(const std::string* pref_name);

  BooleanPrefMember network_prediction_enabled_;
  BooleanPrefMember spdy_disabled_;
  BooleanPrefMember http_throttling_enabled_;
  scoped_refptr<prerender::PrerenderManager> prerender_manager_;

  DISALLOW_COPY_AND_ASSIGN(NetPrefObserver);
};

#endif  

