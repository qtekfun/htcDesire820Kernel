// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_SERVICE_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_SERVICE_H_

#include <vector>

#include "base/strings/string16.h"
#include "url/gurl.h"

namespace content {
class BrowserContext;
}

class BookmarkService {
 public:
  struct URLAndTitle {
    GURL url;
    base::string16 title;
  };

  static BookmarkService* FromBrowserContext(
      content::BrowserContext* browser_context);

  
  
  
  virtual bool IsBookmarked(const GURL& url) = 0;

  
  
  
  
  
  
  virtual void GetBookmarks(std::vector<URLAndTitle>* bookmarks) = 0;

  
  
  virtual void BlockTillLoaded() = 0;

 protected:
  virtual ~BookmarkService() {}
};

#endif  
