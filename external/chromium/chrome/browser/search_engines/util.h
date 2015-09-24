// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_ENGINES_UTIL_H_
#define CHROME_BROWSER_SEARCH_ENGINES_UTIL_H_
#pragma once

#include <vector>

#include "base/string16.h"

class PrefService;
class Profile;
class TemplateURL;
class WDTypedResult;
class WebDataService;

string16 GetDefaultSearchEngineName(Profile* profile);

void GetSearchProvidersUsingKeywordResult(
    const WDTypedResult& result,
    WebDataService* service,
    PrefService* prefs,
    std::vector<TemplateURL*>* template_urls,
    const TemplateURL** default_search_provider,
    int* new_resource_keyword_version);

#endif  
