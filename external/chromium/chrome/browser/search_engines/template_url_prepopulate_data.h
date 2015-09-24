// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_PREPOPULATE_DATA_H_
#define CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_PREPOPULATE_DATA_H_
#pragma once

#include <vector>

class GURL;
class PrefService;
class TemplateURL;

namespace TemplateURLPrepopulateData {

void RegisterUserPrefs(PrefService* prefs);

int GetDataVersion(PrefService* prefs);

void GetPrepopulatedEngines(PrefService* prefs,
                            std::vector<TemplateURL*>* t_urls,
                            size_t* default_search_provider_index);

TemplateURL* GetPrepopulatedDefaultSearch(PrefService* prefs);

TemplateURL* GetEngineForOrigin(PrefService* prefs, const GURL& url_to_find);

int GetSearchEngineLogo(const GURL& url_to_find);

}  

#endif  
