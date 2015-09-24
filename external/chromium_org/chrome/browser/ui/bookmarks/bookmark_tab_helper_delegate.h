// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BOOKMARKS_BOOKMARK_TAB_HELPER_DELEGATE_H_
#define CHROME_BROWSER_UI_BOOKMARKS_BOOKMARK_TAB_HELPER_DELEGATE_H_

#include "base/basictypes.h"

namespace content {
class WebContents;
}

class BookmarkTabHelperDelegate {
 public:
  
  virtual void URLStarredChanged(content::WebContents* web_contents,
                                 bool starred) = 0;

 protected:
  virtual ~BookmarkTabHelperDelegate();
};

#endif  
