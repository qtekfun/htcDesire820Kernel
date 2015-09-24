// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_NET_PREF_OBSERVER_H_
#define CHROME_BROWSER_NET_NET_PREF_OBSERVER_H_

#include "base/basictypes.h"
#include "base/prefs/pref_member.h"

class PrefService;

namespace chrome_browser_net {
class Predictor;
}

namespace prerender {
class PrerenderManager;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class NetPrefObserver {
 public:
  
  
  
  NetPrefObserver(PrefService* prefs,
                  prerender::PrerenderManager* prerender_manager,
                  chrome_browser_net::Predictor* predictor);
  virtual ~NetPrefObserver();

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

 private:
  void ApplySettings();

  BooleanPrefMember network_prediction_enabled_;
  BooleanPrefMember spdy_disabled_;
  prerender::PrerenderManager* prerender_manager_;
  chrome_browser_net::Predictor* predictor_;

  DISALLOW_COPY_AND_ASSIGN(NetPrefObserver);
};

#endif  
