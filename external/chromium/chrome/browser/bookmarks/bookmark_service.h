// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_SERVICE_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_SERVICE_H_
#pragma once

#include <vector>

class GURL;

class BookmarkService {
 public:
  
  
  
  virtual bool IsBookmarked(const GURL& url) = 0;

  
  
  
  
  
  virtual void GetBookmarks(std::vector<GURL>* urls) = 0;

  
  
  virtual void BlockTillLoaded() = 0;

 protected:
  virtual ~BookmarkService() {}
};

#endif  
