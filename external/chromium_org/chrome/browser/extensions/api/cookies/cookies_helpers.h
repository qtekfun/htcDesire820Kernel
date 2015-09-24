// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_API_COOKIES_COOKIES_HELPERS_H_
#define CHROME_BROWSER_EXTENSIONS_API_COOKIES_COOKIES_HELPERS_H_

#include <string>
#include <vector>

#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/common/extensions/api/cookies.h"
#include "net/cookies/cookie_monster.h"
#include "net/cookies/canonical_cookie.h"

class Browser;
class Profile;

namespace base {
class DictionaryValue;
class ListValue;
}

namespace net {
class CanonicalCookie;
}

namespace extensions {

class Extension;

namespace cookies_helpers {

typedef std::vector<linked_ptr<extensions::api::cookies::Cookie> >
    LinkedCookieVec;

Profile* ChooseProfileFromStoreId(const std::string& store_id,
                                  Profile* profile,
                                  bool include_incognito);

const char* GetStoreIdFromProfile(Profile* profile);

scoped_ptr<extensions::api::cookies::Cookie> CreateCookie(
    const net::CanonicalCookie& cookie,
    const std::string& store_id);

scoped_ptr<extensions::api::cookies::CookieStore> CreateCookieStore(
    Profile* profile,
    base::ListValue* tab_ids);

void GetCookieListFromStore(
    net::CookieStore* cookie_store, const GURL& url,
    const net::CookieMonster::GetCookieListCallback& callback);

GURL GetURLFromCanonicalCookie(
    const net::CanonicalCookie& cookie);

void AppendMatchingCookiesToVector(
    const net::CookieList& all_cookies, const GURL& url,
    const extensions::api::cookies::GetAll::Params::Details* details,
    const Extension* extension, LinkedCookieVec* match_vector);

void AppendToTabIdList(Browser* browser, base::ListValue* tab_ids);

class MatchFilter {
 public:
  
  
  
  explicit MatchFilter(
      const extensions::api::cookies::GetAll::Params::Details* details);

  
  
  bool MatchesCookie(const net::CanonicalCookie& cookie);

 private:
  
  
  
  
  
  
  
  bool MatchesDomain(const std::string& domain);

  const extensions::api::cookies::GetAll::Params::Details* details_;
};

}  
}  

#endif  
