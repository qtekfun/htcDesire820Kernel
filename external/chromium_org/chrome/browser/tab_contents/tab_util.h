// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_TAB_UTIL_H_
#define CHROME_BROWSER_TAB_CONTENTS_TAB_UTIL_H_

class GURL;
class Profile;

namespace content {
class SiteInstance;
class WebContents;
}

namespace tab_util {

content::WebContents* GetWebContentsByID(int render_process_id,
                                         int render_view_id);

content::SiteInstance* GetSiteInstanceForNewTab(
    Profile* profile,
    const GURL& url);

}  

#endif  
