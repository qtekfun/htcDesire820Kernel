// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_TITLE_MATCH_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_TITLE_MATCH_H_

#include <stddef.h>

#include <utility>
#include <vector>

class BookmarkNode;

struct BookmarkTitleMatch {
  
  
  typedef std::pair<size_t, size_t> MatchPosition;
  typedef std::vector<MatchPosition> MatchPositions;

  BookmarkTitleMatch();
  ~BookmarkTitleMatch();

  
  const BookmarkNode* node;

  
  MatchPositions match_positions;
};

#endif  
