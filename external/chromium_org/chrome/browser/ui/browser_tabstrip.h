// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BROWSER_TABSTRIP_H_
#define CHROME_BROWSER_UI_BROWSER_TABSTRIP_H_

#include "content/public/browser/navigation_controller.h"
#include "content/public/browser/web_contents.h"
#include "content/public/common/page_transition_types.h"
#include "ui/base/window_open_disposition.h"

class Browser;
class GURL;
class Profile;

namespace content {
class SiteInstance;
}

namespace gfx {
class Rect;
}

namespace chrome {

void AddTabAt(Browser* browser, const GURL& url, int index, bool foreground);

content::WebContents* AddSelectedTabWithURL(Browser* browser,
                                            const GURL& url,
                                            content::PageTransition transition);

void AddWebContents(Browser* browser,
                    content::WebContents* source_contents,
                    content::WebContents* new_contents,
                    WindowOpenDisposition disposition,
                    const gfx::Rect& initial_pos,
                    bool user_gesture,
                    bool* was__blocked);

void CloseWebContents(Browser* browser,
                      content::WebContents* contents,
                      bool add_to_history);

}  

#endif  
