// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GOOGLE_GOOGLE_UTIL_H__
#define CHROME_BROWSER_GOOGLE_GOOGLE_UTIL_H__

#include <string>

#include "base/basictypes.h"

class GURL;
class Profile;

namespace google_util {

bool HasGoogleSearchQueryParam(const std::string& str);

const char kInstantExtendedAPIParam[] = "espv";

GURL LinkDoctorBaseURL();
void SetMockLinkDoctorBaseURLForTesting();

GURL AppendGoogleLocaleParam(const GURL& url);

std::string StringAppendGoogleLocaleParam(const std::string& url);

GURL AppendGoogleTLDParam(Profile* profile, const GURL& url);

bool GetBrand(std::string* brand);

bool GetReactivationBrand(std::string* brand);


enum SubdomainPermission {
  ALLOW_SUBDOMAIN,
  DISALLOW_SUBDOMAIN,
};

enum PortPermission {
  ALLOW_NON_STANDARD_PORTS,
  DISALLOW_NON_STANDARD_PORTS,
};

bool StartsWithCommandLineGoogleBaseURL(const GURL& url);

bool IsGoogleHostname(const std::string& host,
                      SubdomainPermission subdomain_permission);

bool IsGoogleDomainUrl(const GURL& url,
                       SubdomainPermission subdomain_permission,
                       PortPermission port_permission);

bool IsGoogleHomePageUrl(const GURL& url);

bool IsGoogleSearchUrl(const GURL& url);

bool IsOrganic(const std::string& brand);

bool IsOrganicFirstRun(const std::string& brand);

bool IsInternetCafeBrandCode(const std::string& brand);

class BrandForTesting {
 public:
  explicit BrandForTesting(const std::string& brand);
  ~BrandForTesting();

 private:
  std::string brand_;
  DISALLOW_COPY_AND_ASSIGN(BrandForTesting);
};

}  

#endif  
