// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_INTEGRATION_SEARCH_ENGINES_HELPER_H_
#define CHROME_BROWSER_SYNC_TEST_INTEGRATION_SEARCH_ENGINES_HELPER_H_

#include <map>
#include <string>

#include "base/strings/string16.h"

class Profile;
class TemplateURL;
class TemplateURLService;

typedef std::map<std::string, const TemplateURL*> GUIDToTURLMap;

namespace search_engines_helper {

TemplateURLService* GetServiceForBrowserContext(int profile_index);

TemplateURLService* GetVerifierService();

bool ServiceMatchesVerifier(int profile_index);

bool AllServicesMatch();

TemplateURL* CreateTestTemplateURL(Profile* profile,
                                   int seed,
                                   const base::string16& keyword,
                                   const std::string& sync_guid);
TemplateURL* CreateTestTemplateURL(Profile* profile,
                                   int seed,
                                   const base::string16& keyword,
                                   const std::string& url,
                                   const std::string& sync_guid);

void AddSearchEngine(int profile_index, int seed);

void EditSearchEngine(int profile_index,
                      const base::string16& keyword,
                      const base::string16& short_name,
                      const base::string16& new_keyword,
                      const std::string& url);

void DeleteSearchEngineBySeed(int profile_index, int seed);

void ChangeDefaultSearchProvider(int profile_index, int seed);

}  

#endif  
