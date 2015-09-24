// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_FAVICON_UTIL_MAC_H_
#define CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_FAVICON_UTIL_MAC_H_

@class NSImage;

namespace content {
class WebContents;
}

namespace mac {

NSImage* FaviconForWebContents(content::WebContents* contents);

}  

#endif  
