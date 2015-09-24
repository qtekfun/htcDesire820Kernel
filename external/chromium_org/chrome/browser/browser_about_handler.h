// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSER_ABOUT_HANDLER_H_
#define CHROME_BROWSER_BROWSER_ABOUT_HANDLER_H_

class GURL;

namespace content {
class BrowserContext;
}

bool WillHandleBrowserAboutURL(GURL* url,
                               content::BrowserContext* browser_context);

bool HandleNonNavigationAboutURL(const GURL& url);

#endif  
