// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_SEARCH_H_
#define CHROME_BROWSER_SEARCH_SEARCH_H_

#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "chrome/browser/ui/search/search_model.h"

class GURL;
class Profile;
class TemplateURL;
class TemplateURLRef;

namespace content {
class BrowserContext;
class NavigationEntry;
class WebContents;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace chrome {

enum OptInState {
  
  INSTANT_EXTENDED_NOT_SET,
  
  INSTANT_EXTENDED_OPT_IN,
  
  INSTANT_EXTENDED_OPT_OUT,
  INSTANT_EXTENDED_OPT_IN_STATE_ENUM_COUNT,
};

enum DisplaySearchButtonConditions {
  DISPLAY_SEARCH_BUTTON_NEVER,
  DISPLAY_SEARCH_BUTTON_FOR_STR,         
  DISPLAY_SEARCH_BUTTON_FOR_STR_OR_IIP,  
  DISPLAY_SEARCH_BUTTON_ALWAYS,
  DISPLAY_SEARCH_BUTTON_NUM_VALUES,
};

extern const int kDisableStartMargin;

bool IsInstantExtendedAPIEnabled();

bool IsSuggestPrefEnabled(Profile* profile);

uint64 EmbeddedSearchPageVersion();

bool IsQueryExtractionEnabled();

base::string16 GetSearchTermsFromURL(Profile* profile, const GURL& url);

base::string16 GetSearchTermsFromNavigationEntry(
    const content::NavigationEntry* entry);

base::string16 GetSearchTerms(const content::WebContents* contents);

bool ShouldAssignURLToInstantRenderer(const GURL& url, Profile* profile);

bool ShouldUseProcessPerSiteForInstantURL(const GURL& url, Profile* profile);

bool IsNTPURL(const GURL& url, Profile* profile);

bool IsInstantNTP(const content::WebContents* contents);

bool NavEntryIsInstantNTP(const content::WebContents* contents,
                          const content::NavigationEntry* nav_entry);

GURL GetInstantURL(Profile* profile,
                   int start_margin,
                   bool force_instant_results);

std::vector<GURL> GetSearchURLs(Profile* profile);

GURL GetSearchResultPrefetchBaseURL(Profile* profile);

bool ShouldPrefetchSearchResults();

GURL GetLocalInstantURL(Profile* profile);

bool ShouldHideTopVerbatimMatch();

bool ShouldUseCacheableNTP();

bool ShouldShowInstantNTP();

DisplaySearchButtonConditions GetDisplaySearchButtonConditions();

bool ShouldDisplayOriginChip();

GURL GetEffectiveURLForInstant(const GURL& url, Profile* profile);

int GetInstantLoaderStalenessTimeoutSec();

bool IsPreloadedInstantExtendedNTP(const content::WebContents* contents);

// |url| is rewritten to the value of --instant-new-tab-url.
bool HandleNewTabURLRewrite(GURL* url,
                            content::BrowserContext* browser_context);
bool HandleNewTabURLReverseRewrite(GURL* url,
                                   content::BrowserContext* browser_context);

void SetInstantSupportStateInNavigationEntry(InstantSupportState state,
                                             content::NavigationEntry* entry);

InstantSupportState GetInstantSupportStateFromNavigationEntry(
    const content::NavigationEntry& entry);

bool ShouldPrefetchSearchResultsOnSRP();


void EnableQueryExtractionForTesting();

typedef std::vector<std::pair<std::string, std::string> > FieldTrialFlags;

bool GetFieldTrialInfo(FieldTrialFlags* flags);

std::string GetStringValueForFlagWithDefault(const std::string& flag,
                                             const std::string& default_value,
                                             const FieldTrialFlags& flags);

uint64 GetUInt64ValueForFlagWithDefault(const std::string& flag,
                                        uint64 default_value,
                                        const FieldTrialFlags& flags);

bool GetBoolValueForFlagWithDefault(const std::string& flag,
                                    bool default_value,
                                    const FieldTrialFlags& flags);

GURL GetNewTabPageURL(Profile* profile);

}  

#endif  
