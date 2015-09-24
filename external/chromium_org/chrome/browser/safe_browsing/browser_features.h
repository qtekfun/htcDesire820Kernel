// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_BROWSER_FEATURES_H_
#define CHROME_BROWSER_SAFE_BROWSING_BROWSER_FEATURES_H_

namespace safe_browsing {
namespace features {


extern const char kUrlHistoryVisitCount[];

extern const char kUrlHistoryTypedCount[];

extern const char kUrlHistoryLinkCount[];

extern const char kUrlHistoryVisitCountMoreThan24hAgo[];

extern const char kHttpHostVisitCount[];
extern const char kHttpsHostVisitCount[];

extern const char kFirstHttpHostVisitMoreThan24hAgo[];
extern const char kFirstHttpsHostVisitMoreThan24hAgo[];

extern const char kHostPrefix[];

extern const char kReferrer[];
extern const char kHasSSLReferrer[];
extern const char kPageTransitionType[];
extern const char kIsFirstNavigation[];
extern const char kRedirectUrlMismatch[];
extern const char kRedirect[];
extern const char kSecureRedirectValue[];

extern const char kBadIpFetch[];

extern const char kHttpStatusCode[];

extern const char kSafeBrowsingMaliciousUrl[];
extern const char kSafeBrowsingOriginalUrl[];
extern const char kSafeBrowsingIsSubresource[];
extern const char kSafeBrowsingThreatType[];
}  
}  

#endif  
