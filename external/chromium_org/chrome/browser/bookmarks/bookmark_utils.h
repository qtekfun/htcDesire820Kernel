// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_UTILS_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_UTILS_H_

#include <string>
#include <vector>

#include "base/strings/string16.h"
#include "chrome/browser/bookmarks/bookmark_node_data.h"

class BookmarkModel;
class BookmarkNode;

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace bookmark_utils {

void CloneBookmarkNode(BookmarkModel* model,
                       const std::vector<BookmarkNodeData::Element>& elements,
                       const BookmarkNode* parent,
                       int index_to_add_at,
                       bool reset_node_times);

void CopyToClipboard(BookmarkModel* model,
                     const std::vector<const BookmarkNode*>& nodes,
                     bool remove_nodes);

void PasteFromClipboard(BookmarkModel* model,
                        const BookmarkNode* parent,
                        int index);

bool CanPasteFromClipboard(const BookmarkNode* node);

std::vector<const BookmarkNode*> GetMostRecentlyModifiedFolders(
    BookmarkModel* model, size_t max_count);

void GetMostRecentlyAddedEntries(BookmarkModel* model,
                                 size_t count,
                                 std::vector<const BookmarkNode*>* nodes);

bool MoreRecentlyAdded(const BookmarkNode* n1, const BookmarkNode* n2);

void GetBookmarksContainingText(BookmarkModel* model,
                                const base::string16& text,
                                size_t max_count,
                                const std::string& languages,
                                std::vector<const BookmarkNode*>* nodes);

void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

const BookmarkNode* GetParentForNewNodes(
    const BookmarkNode* parent,
    const std::vector<const BookmarkNode*>& selection,
    int* index);

void DeleteBookmarkFolders(BookmarkModel* model, const std::vector<int64>& ids);

void AddIfNotBookmarked(BookmarkModel* model,
                        const GURL& url,
                        const base::string16& title);

void RemoveAllBookmarks(BookmarkModel* model, const GURL& url);

}  

#endif  
