// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_INCOGNITO_HELPERS_H_
#define CHROME_BROWSER_PROFILES_INCOGNITO_HELPERS_H_

namespace content {
class BrowserContext;
}

namespace chrome {

content::BrowserContext* GetBrowserContextRedirectedInIncognito(
    content::BrowserContext* context);

content::BrowserContext* GetBrowserContextOwnInstanceInIncognito(
    content::BrowserContext* context);

}  

#endif  
