// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_COOKIES_HELPERS_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_COOKIES_HELPERS_H_
#pragma once

#include <string>

#include "net/base/cookie_monster.h"

class Browser;
class DictionaryValue;
class Extension;
class ListValue;
class Profile;

namespace extension_cookies_helpers {

Profile* ChooseProfileFromStoreId(const std::string& store_id,
                                  Profile* profile,
                                  bool include_incognito);

const char* GetStoreIdFromProfile(Profile* profile);

DictionaryValue* CreateCookieValue(
    const net::CookieMonster::CanonicalCookie& cookie,
    const std::string& store_id);

DictionaryValue* CreateCookieStoreValue(Profile* profile,
                                        ListValue* tab_ids);

net::CookieList GetCookieListFromStore(
    net::CookieStore* cookie_store, const GURL& url);

GURL GetURLFromCanonicalCookie(
    const net::CookieMonster::CanonicalCookie& cookie);

void AppendMatchingCookiesToList(
    const net::CookieList& all_cookies,
    const std::string& store_id,
    const GURL& url, const DictionaryValue* details,
    const Extension* extension,
    ListValue* match_list);

void AppendToTabIdList(Browser* browser, ListValue* tab_ids);

class MatchFilter {
 public:
  
  
  
  explicit MatchFilter(const DictionaryValue* details);

  
  
  bool MatchesCookie(const net::CookieMonster::CanonicalCookie& cookie);

 private:
  
  
  
  bool MatchesString(const char* key, const std::string& value);

  
  
  
  bool MatchesBoolean(const char* key, bool value);

  
  
  
  
  
  
  
  bool MatchesDomain(const std::string& domain);

  const DictionaryValue* details_;
};

}  

#endif  
