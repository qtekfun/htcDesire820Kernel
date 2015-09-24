// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_BUBBLE_VIEW_H_
#pragma once

#include "base/string16.h"
#include "chrome/browser/bookmarks/recently_used_folders_combo_model.h"
#include "chrome/browser/ui/views/bubble/bubble.h"
#include "googleurl/src/gurl.h"
#include "ui/gfx/rect.h"
#include "views/controls/button/button.h"
#include "views/controls/combobox/combobox.h"
#include "views/controls/link.h"
#include "views/view.h"

class Profile;

class BookmarkModel;
class BookmarkNode;

namespace views {
class NativeButton;
class Textfield;
}

class BookmarkBubbleView : public views::View,
                           public views::LinkController,
                           public views::ButtonListener,
                           public views::Combobox::Listener,
                           public BubbleDelegate {
 public:
  static void Show(views::Window* window,
                   const gfx::Rect& bounds,
                   BubbleDelegate* delegate,
                   Profile* profile,
                   const GURL& url,
                   bool newly_bookmarked);

  static bool IsShowing();

  static void Hide();

  virtual ~BookmarkBubbleView();

  void set_bubble(Bubble* bubble) { bubble_ = bubble; }

  
  virtual void BubbleShown();

  
  virtual bool AcceleratorPressed(const views::Accelerator& accelerator);

  virtual void ViewHierarchyChanged(bool is_add, View* parent, View* child);

 private:
  
  
  
  BookmarkBubbleView(BubbleDelegate* delegate,
                     Profile* profile,
                     const GURL& url,
                     bool newly_bookmarked);
  
  void Init();

  
  string16 GetTitle();

  
  
  virtual void LinkActivated(views::Link* source, int event_flags);

  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event);

  
  virtual void ItemChanged(views::Combobox* combobox,
                           int prev_index,
                           int new_index);

  
  
  virtual void BubbleClosing(Bubble* bubble, bool closed_by_escape);
  virtual bool CloseOnEscape();
  virtual bool FadeInOnShow();
  virtual std::wstring accessible_name();

  
  void Close();

  
  void HandleButtonPressed(views::Button* sender);

  
  void ShowEditor();

  
  void ApplyEdits();

  
  static BookmarkBubbleView* bookmark_bubble_;

  
  Bubble* bubble_;

  
  BubbleDelegate* delegate_;

  
  Profile* profile_;

  
  const GURL url_;

  
  
  std::wstring title_;

  
  const bool newly_bookmarked_;

  RecentlyUsedFoldersComboModel parent_model_;

  
  views::Link* remove_link_;

  
  views::NativeButton* edit_button_;

  
  views::NativeButton* close_button_;

  
  views::Textfield* title_tf_;

  
  
  views::Combobox* parent_combobox_;

  
  bool remove_bookmark_;

  
  bool apply_edits_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkBubbleView);
};

#endif  
