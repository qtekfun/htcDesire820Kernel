// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_TEST_HELPERS_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_TEST_HELPERS_H_

#include <string>

class BookmarkModel;
class BookmarkNode;
class Profile;

namespace test {

void WaitForBookmarkModelToLoad(BookmarkModel* model);
void WaitForBookmarkModelToLoad(Profile* profile);

std::string ModelStringFromNode(const BookmarkNode* node);

void AddNodesFromModelString(BookmarkModel* model,
                             const BookmarkNode* node,
                             const std::string& model_string);
}  

#endif  
