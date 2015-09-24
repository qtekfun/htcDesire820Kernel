// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRERENDER_PRERENDER_FIELD_TRIAL_H_
#define CHROME_BROWSER_PRERENDER_PRERENDER_FIELD_TRIAL_H_

#include <string>

class CommandLine;
class Profile;

namespace prerender {

void ConfigurePrerender(const CommandLine& command_line);

bool IsOmniboxEnabled(Profile* profile);

bool ShouldMergeSessionStorageNamespaces();

bool IsLocalPredictorEnabled();

bool DisableLocalPredictorBasedOnSyncAndConfiguration(Profile* profile);

bool IsLoggedInPredictorEnabled();

bool IsSideEffectFreeWhitelistEnabled();

bool IsLocalPredictorPrerenderLaunchEnabled();

bool IsLocalPredictorPrerenderAlwaysControlEnabled();

bool ShouldQueryPrerenderService(Profile* profile);

bool ShouldQueryPrerenderServiceForCurrentURL();
bool ShouldQueryPrerenderServiceForCandidateURLs();

std::string GetPrerenderServiceURLPrefix();

int GetPrerenderServiceBehaviorID();

int GetPrerenderServiceFetchTimeoutMs();

int GetLocalPredictorTTLSeconds();

int GetLocalPredictorPrerenderPriorityHalfLifeTimeSeconds();

int GetLocalPredictorMaxConcurrentPrerenders();

bool SkipLocalPredictorFragment();
bool SkipLocalPredictorHTTPS();
bool SkipLocalPredictorWhitelist();
bool SkipLocalPredictorServiceWhitelist();
bool SkipLocalPredictorLoggedIn();
bool SkipLocalPredictorDefaultNoPrerender();
bool SkipLocalPredictorLocalCandidates();
bool SkipLocalPredictorServiceCandidates();

}  

#endif  
