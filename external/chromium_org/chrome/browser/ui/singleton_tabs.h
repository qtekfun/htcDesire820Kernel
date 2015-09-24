// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SINGLETON_TABS_H_
#define CHROME_BROWSER_UI_SINGLETON_TABS_H_

class Browser;
class GURL;


namespace chrome {

struct NavigateParams;


void ShowSingletonTab(Browser* browser, const GURL& url);

void ShowSingletonTabRespectRef(Browser* browser, const GURL& url);

void ShowSingletonTabOverwritingNTP(Browser* browser,
                                    const NavigateParams& params);

NavigateParams GetSingletonTabNavigateParams(Browser* browser, const GURL& url);

int GetIndexOfSingletonTab(NavigateParams* params);

}  

#endif  
