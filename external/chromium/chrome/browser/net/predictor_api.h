// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_NET_PREDICTOR_API_H_
#define CHROME_BROWSER_NET_PREDICTOR_API_H_
#pragma once


#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "chrome/browser/autocomplete/autocomplete.h"
#include "chrome/browser/net/predictor.h"

namespace base {
class FieldTrial;
}

class PrefService;

namespace chrome_browser_net {

void FinalizePredictorInitialization(
    Predictor* global_predictor,
    const std::vector<GURL>& urls_to_prefetch,
    ListValue* referral_list);

void FreePredictorResources();

void EnablePredictor(bool enable);
void DiscardInitialNavigationHistory();
void RegisterUserPrefs(PrefService* user_prefs);

void DnsPrefetchList(const NameList& hostnames);

void AnticipateOmniboxUrl(const GURL& url, bool preconnectable);

void PreconnectUrlAndSubresources(const GURL& url);

void PredictorGetHtmlInfo(std::string* output);

void ClearPredictorCache();

void PredictFrameSubresources(const GURL& url);

void LearnAboutInitialNavigation(const GURL& url);

void LearnFromNavigation(const GURL& referring_url, const GURL& target_url);

void SavePredictorStateForNextStartupAndTrim(PrefService* prefs);
class PredictorInit {
 public:
  
  
  static const size_t kMaxSpeculativeParallelResolves;

  
  
  
  static const int kMaxSpeculativeResolveQueueDelayMs;

  PredictorInit(PrefService* user_prefs, PrefService* local_state,
                bool preconnect_enabled);
  ~PredictorInit();

 private:
  
  scoped_refptr<base::FieldTrial> trial_;

  DISALLOW_COPY_AND_ASSIGN(PredictorInit);
};

}  

#endif  
