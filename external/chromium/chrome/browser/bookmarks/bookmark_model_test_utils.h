// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_MODEL_TEST_UTILS_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_MODEL_TEST_UTILS_H_
#pragma once

class BookmarkModel;
class BookmarkNode;

class BookmarkModelTestUtils {
 public:
  
  
  static void AssertModelsEqual(BookmarkModel* expected,
                                BookmarkModel* actual,
                                bool check_ids);
 private:
  
  
  static void AssertNodesEqual(const BookmarkNode* expected,
                               const BookmarkNode* actual,
                               bool check_ids);
};

#endif  
