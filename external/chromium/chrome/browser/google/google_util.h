// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GOOGLE_GOOGLE_UTIL_H__
#define CHROME_BROWSER_GOOGLE_GOOGLE_UTIL_H__
#pragma once

#include <string>

class GURL;

namespace google_util {

extern const char kLinkDoctorBaseURL[];

GURL AppendGoogleLocaleParam(const GURL& url);

std::string StringAppendGoogleLocaleParam(const std::string& url);

GURL AppendGoogleTLDParam(const GURL& url);

}  

#endif  
