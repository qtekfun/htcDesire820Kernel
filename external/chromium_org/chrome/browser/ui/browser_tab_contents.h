// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BROWSER_TAB_CONTENTS_H_
#define CHROME_BROWSER_UI_BROWSER_TAB_CONTENTS_H_

class Browser;

namespace content {
class WebContents;
}

namespace chrome {
class BrowserTabStripModelDelegate;
}

namespace prerender {
class PrerenderContents;
}

class BrowserTabContents {
 private:
  
  
  friend class Browser;
  friend class chrome::BrowserTabStripModelDelegate;
  friend class TabAndroid;

  
  
  
  friend class BrowserNavigatorWebContentsAdoption;

  
  
  
  
  
  
  friend class ExternalTabContainerWin;

  
  
  friend class prerender::PrerenderContents;

  
  
  
  
  static void AttachTabHelpers(content::WebContents* web_contents);
};

#endif  
