// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Copyright (c) 2011, Code Aurora Forum. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_PRECONNECT_H_
#define CHROME_BROWSER_NET_PRECONNECT_H_
#pragma once

#include "net/http/preconnect.h"

#include "chrome/browser/net/url_info.h"

namespace chrome_browser_net {

void PreconnectOnUIThread(const GURL& url,
                          UrlInfo::ResolutionMotivation motivation,
                          int count);

void PreconnectOnIOThread(const GURL& url,
                          UrlInfo::ResolutionMotivation motivation,
                          int count);
}  

#endif  
