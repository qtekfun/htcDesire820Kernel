// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INSTANT_PROMO_COUNTER_H_
#define CHROME_BROWSER_INSTANT_PROMO_COUNTER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/time.h"

class PrefService;
class Profile;

class PromoCounter {
 public:
  
  
  
  
  
  PromoCounter(Profile* profile,
               const std::string& pref_key,
               const std::string& histogram_key,
               int max_sessions,
               int max_days);
  ~PromoCounter();

  
  static void RegisterUserPrefs(PrefService* prefs,
                                const std::string& base_key);

  
  bool ShouldShow(base::Time current_time);

  
  void Hide();

 private:
  
  
  void Init(base::Time current_time);

  
  void MaxSessionsEncountered(base::Time current_time);

  
  void MaxTimeLapsed(base::Time current_time);

  Profile* profile_;

  
  const std::string pref_key_;

  
  const std::string histogram_key_;

  
  const int max_sessions_;
  const int max_days_;

  
  bool did_init_;

  
  bool show_;

  
  base::Time initial_show_;

  DISALLOW_COPY_AND_ASSIGN(PromoCounter);
};

#endif  
