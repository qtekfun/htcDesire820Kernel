// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_PREPOPULATE_DATA_H_
#define CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_PREPOPULATE_DATA_H_

#include <stddef.h>
#include <string>
#include <vector>

#include "base/memory/scoped_vector.h"
#include "base/strings/string16.h"
#include "chrome/browser/search_engines/search_engine_type.h"

class GURL;
class PrefService;
class Profile;
class TemplateURL;

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace TemplateURLPrepopulateData {

extern const int kMaxPrepopulatedEngineID;

enum LogoSize {
  LOGO_100_PERCENT,
  LOGO_200_PERCENT,
};

#if defined(OS_ANDROID)
void InitCountryCode(const std::string& country_code);
#endif

void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

int GetDataVersion(PrefService* prefs);

ScopedVector<TemplateURL> GetPrepopulatedEngines(
    Profile* profile, size_t* default_search_provider_index);

void ClearPrepopulatedEnginesInPrefs(Profile* profile);

TemplateURL* GetPrepopulatedDefaultSearch(Profile* profile);

SearchEngineType GetEngineType(const TemplateURL& template_url);

SearchEngineType GetEngineType(const GURL& url);

GURL GetLogoURL(const TemplateURL& template_url, LogoSize size);

}  

#endif  
