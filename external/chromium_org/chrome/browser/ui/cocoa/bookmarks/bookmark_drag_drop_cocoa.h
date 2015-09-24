// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_DRAG_DROP_COCOA_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_DRAG_DROP_COCOA_H_

#include "base/strings/string16.h"

#if defined(__OBJC__)
@class NSImage;
#else  
class NSImage;
#endif  

namespace chrome {

NSImage* DragImageForBookmark(NSImage* favicon, const base::string16& title);

}  

#endif  
