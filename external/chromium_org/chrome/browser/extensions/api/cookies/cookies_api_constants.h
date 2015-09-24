// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_API_COOKIES_COOKIES_API_CONSTANTS_H_
#define CHROME_BROWSER_EXTENSIONS_API_COOKIES_COOKIES_API_CONSTANTS_H_

namespace extensions {
namespace cookies_api_constants {

extern const char kCauseKey[];
extern const char kCookieKey[];
extern const char kDomainKey[];
extern const char kIdKey[];
extern const char kRemovedKey[];
extern const char kTabIdsKey[];

extern const char kEvictedChangeCause[];
extern const char kExpiredChangeCause[];
extern const char kExpiredOverwriteChangeCause[];
extern const char kExplicitChangeCause[];
extern const char kOverwriteChangeCause[];

extern const char kOnChanged[];

extern const char kCookieSetFailedError[];
extern const char kInvalidStoreIdError[];
extern const char kInvalidUrlError[];
extern const char kNoCookieStoreFoundError[];
extern const char kNoHostPermissionsError[];

}  
}  

#endif  
