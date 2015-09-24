// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_STATS_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_STATS_H_

class BookmarkNode;

enum BookmarkEntryPoint {
  BOOKMARK_ENTRY_POINT_ACCELERATOR,
  BOOKMARK_ENTRY_POINT_STAR_GESTURE,
  BOOKMARK_ENTRY_POINT_STAR_KEY,
  BOOKMARK_ENTRY_POINT_STAR_MOUSE,

  BOOKMARK_ENTRY_POINT_LIMIT 
};

enum BookmarkLaunchLocation {
  BOOKMARK_LAUNCH_LOCATION_NONE,
  BOOKMARK_LAUNCH_LOCATION_ATTACHED_BAR = 0,
  BOOKMARK_LAUNCH_LOCATION_DETACHED_BAR,
  
  
  
  
  BOOKMARK_LAUNCH_LOCATION_BAR_SUBFOLDER,
  BOOKMARK_LAUNCH_LOCATION_CONTEXT_MENU,

  
  BOOKMARK_LAUNCH_LOCATION_WRENCH_MENU,
  
  BOOKMARK_LAUNCH_LOCATION_MANAGER,
  
  BOOKMARK_LAUNCH_LOCATION_OMNIBOX,

  BOOKMARK_LAUNCH_LOCATION_LIMIT  
};

void RecordBookmarkLaunch(const BookmarkNode* node,
                          BookmarkLaunchLocation location);

void RecordBookmarkFolderOpen(BookmarkLaunchLocation location);

void RecordBookmarkAppsPageOpen(BookmarkLaunchLocation location);

#endif  
