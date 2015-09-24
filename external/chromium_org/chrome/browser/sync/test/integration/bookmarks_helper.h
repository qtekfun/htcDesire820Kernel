// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_INTEGRATION_BOOKMARKS_HELPER_H_
#define CHROME_BROWSER_SYNC_TEST_INTEGRATION_BOOKMARKS_HELPER_H_

#include <set>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "chrome/browser/bookmarks/bookmark_model.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "url/gurl.h"

class GURL;

namespace bookmarks_helper {

BookmarkModel* GetBookmarkModel(int index) WARN_UNUSED_RESULT;

const BookmarkNode* GetBookmarkBarNode(int index) WARN_UNUSED_RESULT;

const BookmarkNode* GetOtherNode(int index) WARN_UNUSED_RESULT;

const BookmarkNode* GetSyncedBookmarksNode(int index) WARN_UNUSED_RESULT;

BookmarkModel* GetVerifierBookmarkModel() WARN_UNUSED_RESULT;

const BookmarkNode* AddURL(
    int profile,
    const std::wstring& title,
    const GURL& url) WARN_UNUSED_RESULT;

const BookmarkNode* AddURL(
    int profile,
    int index,
    const std::wstring& title,
    const GURL& url) WARN_UNUSED_RESULT;

const BookmarkNode* AddURL(
    int profile,
    const BookmarkNode* parent,
    int index,
    const std::wstring& title,
    const GURL& url) WARN_UNUSED_RESULT;

const BookmarkNode* AddFolder(
    int profile,
    const std::wstring& title) WARN_UNUSED_RESULT;

const BookmarkNode* AddFolder(
    int profile,
    int index,
    const std::wstring& title) WARN_UNUSED_RESULT;

const BookmarkNode* AddFolder(
    int profile,
    const BookmarkNode* parent,
    int index,
    const std::wstring& title) WARN_UNUSED_RESULT;

void SetTitle(int profile,
                     const BookmarkNode* node,
                     const std::wstring& new_title);

enum FaviconSource {
  FROM_UI,
  FROM_SYNC
};

void SetFavicon(int profile,
                const BookmarkNode* node,
                const GURL& icon_url,
                const gfx::Image& image,
                FaviconSource source);

const BookmarkNode* SetURL(
    int profile,
    const BookmarkNode* node,
    const GURL& new_url) WARN_UNUSED_RESULT;

void Move(
    int profile,
    const BookmarkNode* node,
    const BookmarkNode* new_parent,
    int index);

void Remove(int profile, const BookmarkNode* parent, int index);

void RemoveAll(int profile);

void SortChildren(int profile, const BookmarkNode* parent);

void ReverseChildOrder(int profile, const BookmarkNode* parent);

bool ModelMatchesVerifier(int profile) WARN_UNUSED_RESULT;

bool AllModelsMatchVerifier() WARN_UNUSED_RESULT;

bool ModelsMatch(int profile_a, int profile_b) WARN_UNUSED_RESULT;

bool AllModelsMatch() WARN_UNUSED_RESULT;

bool ContainsDuplicateBookmarks(int profile);

bool HasNodeWithURL(int profile, const GURL& url);

const BookmarkNode* GetUniqueNodeByURL(
    int profile,
    const GURL& url) WARN_UNUSED_RESULT;

int CountBookmarksWithTitlesMatching(
    int profile,
    const std::wstring& title) WARN_UNUSED_RESULT;

int CountFoldersWithTitlesMatching(
    int profile,
    const std::wstring& title) WARN_UNUSED_RESULT;

gfx::Image CreateFavicon(SkColor color);

gfx::Image Create1xFaviconFromPNGFile(const std::string& path);

std::string IndexedURL(int i);

std::wstring IndexedURLTitle(int i);

std::wstring IndexedFolderName(int i);

std::wstring IndexedSubfolderName(int i);

std::wstring IndexedSubsubfolderName(int i);

}  

#endif  
