// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_FAVICON_UTIL_H_
#define CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_FAVICON_UTIL_H_

@class NSImage;
class TabContents;

namespace mac {

NSImage* FaviconForTabContents(TabContents* contents);

}  

#endif  
