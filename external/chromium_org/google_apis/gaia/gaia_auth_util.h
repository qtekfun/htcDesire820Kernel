// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GAIA_GAIA_AUTH_UTIL_H_
#define GOOGLE_APIS_GAIA_GAIA_AUTH_UTIL_H_

#include <string>
#include <vector>

class GURL;

namespace gaia {

std::string CanonicalizeEmail(const std::string& email_address);

std::string CanonicalizeDomain(const std::string& domain);

std::string SanitizeEmail(const std::string& email_address);

bool AreEmailsSame(const std::string& email1, const std::string& email2);

std::string ExtractDomainName(const std::string& email);

bool IsGaiaSignonRealm(const GURL& url);

std::vector<std::string> ParseListAccountsData(const std::string& data);

}  

#endif  
