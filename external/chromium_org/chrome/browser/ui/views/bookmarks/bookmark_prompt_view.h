// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_PROMPT_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_PROMPT_VIEW_H_

#include "base/basictypes.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/link_listener.h"

class PrefService;

class BookmarkPromptView : public views::BubbleDelegateView,
                           public views::LinkListener {
 public:
  
  static void ShowPrompt(views::View* anchor_view, PrefService* prefs);

 private:
  
  BookmarkPromptView(views::View* anchor_view, PrefService* prefs);

  virtual ~BookmarkPromptView();

  
  virtual void Init() OVERRIDE;

  
  virtual void LinkClicked(views::Link* source, int event_flags) OVERRIDE;

  
  virtual void WindowClosing() OVERRIDE;

  
  static BookmarkPromptView* bookmark_bubble_;

  
  views::Link* dismiss_link_;

  
  PrefService* prefs_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkPromptView);
};

#endif  
