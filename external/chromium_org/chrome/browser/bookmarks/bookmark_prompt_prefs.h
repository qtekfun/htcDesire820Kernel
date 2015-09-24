// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_PROMPT_PREFS_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_PROMPT_PREFS_H_

#include "base/basictypes.h"

class PrefService;

namespace user_prefs {
class PrefRegistrySyncable;
}

class BookmarkPromptPrefs {
 public:
  
  
  explicit BookmarkPromptPrefs(PrefService* prefs);
  ~BookmarkPromptPrefs();

  
  void DisableBookmarkPrompt();

  
  int GetPromptImpressionCount() const;

  
  void IncrementPromptImpressionCount();

  
  bool IsBookmarkPromptEnabled() const;

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

 private:
  PrefService* prefs_;  

  DISALLOW_COPY_AND_ASSIGN(BookmarkPromptPrefs);
};

#endif  
