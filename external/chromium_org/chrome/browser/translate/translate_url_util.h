// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TRANSLATE_TRANSLATE_URL_UTIL_H_
#define CHROME_BROWSER_TRANSLATE_TRANSLATE_URL_UTIL_H_

#include "url/gurl.h"

namespace TranslateURLUtil {

GURL AddApiKeyToUrl(const GURL& url);

GURL AddHostLocaleToUrl(const GURL& url);

}  

#endif  
