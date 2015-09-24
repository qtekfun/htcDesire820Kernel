// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_SIGNIN_PROMO_H_
#define CHROME_BROWSER_SIGNIN_SIGNIN_PROMO_H_

#include <string>

#include "base/basictypes.h"

class GURL;
class Profile;

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace signin {

enum Source {
  SOURCE_START_PAGE = 0, 
  SOURCE_NTP_LINK,
  SOURCE_MENU,
  SOURCE_SETTINGS,
  SOURCE_EXTENSION_INSTALL_BUBBLE,
  SOURCE_WEBSTORE_INSTALL,
  SOURCE_APP_LAUNCHER,
  SOURCE_APPS_PAGE_LINK,
  SOURCE_BOOKMARK_BUBBLE,
  SOURCE_AVATAR_BUBBLE_SIGN_IN,
  SOURCE_AVATAR_BUBBLE_ADD_ACCOUNT,
  SOURCE_UNKNOWN, 
};

bool ShouldShowPromo(Profile* profile);

bool ShouldShowPromoAtStartup(Profile* profile, bool is_new_profile);

void DidShowPromoAtStartup(Profile* profile);

void SetUserSkippedPromo(Profile* profile);

GURL GetLandingURL(const char* option, int value);

GURL GetPromoURL(Source source, bool auto_close);
GURL GetPromoURL(Source source, bool auto_close, bool is_constrained);

GURL GetNextPageURLForPromoURL(const GURL& url);

Source GetSourceForPromoURL(const GURL& url);

bool IsAutoCloseEnabledInURL(const GURL& url);

bool IsContinueUrlForWebBasedSigninFlow(const GURL& url);

void ForceWebBasedSigninFlowForTesting(bool force);

void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

}  

#endif  
