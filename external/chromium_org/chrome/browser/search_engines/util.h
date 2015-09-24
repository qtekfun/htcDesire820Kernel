// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_ENGINES_UTIL_H_
#define CHROME_BROWSER_SEARCH_ENGINES_UTIL_H_

#include <set>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/search_engines/template_url_service.h"

class Profile;
class TemplateURL;
class WDTypedResult;
class WebDataService;

base::string16 GetDefaultSearchEngineName(Profile* profile);

GURL GetDefaultSearchURLForSearchTerms(Profile* profile,
                                       const base::string16& terms);

TemplateURL* FindURLByPrepopulateID(
    const TemplateURLService::TemplateURLVector& template_urls,
    int prepopulate_id);

void MergeIntoPrepopulatedEngineData(TemplateURLData* prepopulated_url,
                                     const TemplateURL* original_turl);


typedef std::pair<TemplateURL*, TemplateURLData> EditedSearchEngine;
typedef std::vector<EditedSearchEngine> EditedEngines;

struct ActionsFromPrepopulateData {
  ActionsFromPrepopulateData();
  ~ActionsFromPrepopulateData();

  TemplateURLService::TemplateURLVector removed_engines;
  EditedEngines edited_engines;
  TemplateURLService::TemplateURLVector added_engines;
};

ActionsFromPrepopulateData CreateActionsFromCurrentPrepopulateData(
    ScopedVector<TemplateURL>* prepopulated_urls,
    const TemplateURLService::TemplateURLVector& existing_urls,
    const TemplateURL* default_search_provider);

void GetSearchProvidersUsingKeywordResult(
    const WDTypedResult& result,
    WebDataService* service,
    Profile* profile,
    TemplateURLService::TemplateURLVector* template_urls,
    TemplateURL** default_search_provider,
    int* new_resource_keyword_version,
    std::set<std::string>* removed_keyword_guids);

void GetSearchProvidersUsingLoadedEngines(
    WebDataService* service,
    Profile* profile,
    TemplateURLService::TemplateURLVector* template_urls,
    TemplateURL** default_search_provider,
    int* resource_keyword_version,
    std::set<std::string>* removed_keyword_guids);

bool DeDupeEncodings(std::vector<std::string>* encodings);

void RemoveDuplicatePrepopulateIDs(
    WebDataService* service,
    const ScopedVector<TemplateURL>& prepopulated_urls,
    TemplateURL* default_search_provider,
    TemplateURLService::TemplateURLVector* template_urls,
    std::set<std::string>* removed_keyword_guids);

#endif  
