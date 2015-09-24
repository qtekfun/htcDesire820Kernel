// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_URL_UTILS_H_
#define CHROME_BROWSER_HISTORY_URL_UTILS_H_

#include <string>

#include "chrome/browser/history/history_types.h"

namespace history {

bool CanonicalURLStringCompare(const std::string& s1, const std::string& s2);

bool HaveSameSchemeHostAndPort(const GURL&url1, const GURL& url2);

bool IsPathPrefix(const std::string& p1, const std::string& p2);

GURL ToggleHTTPAndHTTPS(const GURL& url);

}  

#endif  
