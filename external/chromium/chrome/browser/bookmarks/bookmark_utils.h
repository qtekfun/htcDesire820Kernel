// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_UTILS_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_UTILS_H_
#pragma once

#include <string>
#include <vector>

#include "base/string16.h"
#include "chrome/browser/bookmarks/bookmark_editor.h"
#include "chrome/browser/bookmarks/bookmark_node_data.h"
#include "chrome/browser/history/snippet.h"
#include "ui/gfx/native_widget_types.h"
#include "webkit/glue/window_open_disposition.h"

class BookmarkModel;
class BookmarkNode;
class Browser;
class PageNavigator;
class PrefService;
class Profile;
class TabContents;

namespace views {
class DropTargetEvent;
}

namespace bookmark_utils {

int PreferredDropOperation(int source_operations, int operations);

int BookmarkDragOperation(Profile* profile, const BookmarkNode* node);

int BookmarkDropOperation(Profile* profile,
                          const views::DropTargetEvent& event,
                          const BookmarkNodeData& data,
                          const BookmarkNode* parent,
                          int index);

int PerformBookmarkDrop(Profile* profile,
                        const BookmarkNodeData& data,
                        const BookmarkNode* parent_node,
                        int index);

bool IsValidDropLocation(Profile* profile,
                         const BookmarkNodeData& data,
                         const BookmarkNode* drop_parent,
                         int index);

void CloneBookmarkNode(BookmarkModel* model,
                       const std::vector<BookmarkNodeData::Element>& elements,
                       const BookmarkNode* parent,
                       int index_to_add_at);

void DragBookmarks(Profile* profile,
                   const std::vector<const BookmarkNode*>& nodes,
                   gfx::NativeView view);

void OpenAll(gfx::NativeWindow parent,
             Profile* profile,
             PageNavigator* navigator,
             const std::vector<const BookmarkNode*>& nodes,
             WindowOpenDisposition initial_disposition);

void OpenAll(gfx::NativeWindow parent,
             Profile* profile,
             PageNavigator* navigator,
             const BookmarkNode* node,
             WindowOpenDisposition initial_disposition);

void CopyToClipboard(BookmarkModel* model,
                     const std::vector<const BookmarkNode*>& nodes,
                     bool remove_nodes);

void PasteFromClipboard(BookmarkModel* model,
                        const BookmarkNode* parent,
                        int index);

bool CanPasteFromClipboard(const BookmarkNode* node);

string16 GetNameForURL(const GURL& url);

std::vector<const BookmarkNode*> GetMostRecentlyModifiedFolders(
    BookmarkModel* model, size_t max_count);

void GetMostRecentlyAddedEntries(BookmarkModel* model,
                                 size_t count,
                                 std::vector<const BookmarkNode*>* nodes);

struct TitleMatch {
  TitleMatch();
  ~TitleMatch();

  const BookmarkNode* node;

  
  Snippet::MatchPositions match_positions;
};

bool MoreRecentlyAdded(const BookmarkNode* n1, const BookmarkNode* n2);

void GetBookmarksContainingText(BookmarkModel* model,
                                const string16& text,
                                size_t max_count,
                                const std::string& languages,
                                std::vector<const BookmarkNode*>* nodes);

bool DoesBookmarkContainText(const BookmarkNode* node,
                             const string16& text,
                             const std::string& languages);

const BookmarkNode* ApplyEditsWithNoFolderChange(
    BookmarkModel* model,
    const BookmarkNode* parent,
    const BookmarkEditor::EditDetails& details,
    const string16& new_title,
    const GURL& new_url);

const BookmarkNode* ApplyEditsWithPossibleFolderChange(
    BookmarkModel* model,
    const BookmarkNode* new_parent,
    const BookmarkEditor::EditDetails& details,
    const string16& new_title,
    const GURL& new_url);

void ToggleWhenVisible(Profile* profile);

void RegisterUserPrefs(PrefService* prefs);

void GetURLAndTitleToBookmark(TabContents* tab_contents,
                              GURL* url,
                              string16* title);

void GetURLsForOpenTabs(Browser* browser,
                        std::vector<std::pair<GURL, string16> >* urls);

const BookmarkNode* GetParentForNewNodes(
    const BookmarkNode* parent,
    const std::vector<const BookmarkNode*>& selection,
    int* index);

bool NodeHasURLs(const BookmarkNode* node);

extern int num_urls_before_prompting;

}  

#endif  
