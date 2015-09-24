// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_BUBBLE_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/ui/bookmarks/bookmark_bubble_delegate.h"
#include "chrome/browser/ui/bookmarks/recently_used_folders_combo_model.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/combobox/combobox_listener.h"
#include "url/gurl.h"

class BookmarkBubbleViewObserver;
class Profile;

namespace views {
class LabelButton;
class Textfield;
}

class BookmarkBubbleView : public views::BubbleDelegateView,
                           public views::ButtonListener,
                           public views::ComboboxListener {
 public:
  static void ShowBubble(views::View* anchor_view,
                         BookmarkBubbleViewObserver* observer,
                         scoped_ptr<BookmarkBubbleDelegate> delegate,
                         Profile* profile,
                         const GURL& url,
                         bool newly_bookmarked);

  static bool IsShowing();

  static void Hide();

  virtual ~BookmarkBubbleView();

  
  virtual views::View* GetInitiallyFocusedView() OVERRIDE;

  
  virtual void WindowClosing() OVERRIDE;

  
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;

 protected:
  
  virtual void Init() OVERRIDE;

 private:
  friend class BookmarkBubbleViewTest;
  FRIEND_TEST_ALL_PREFIXES(BookmarkBubbleViewTest, SyncPromoSignedIn);
  FRIEND_TEST_ALL_PREFIXES(BookmarkBubbleViewTest, SyncPromoNotSignedIn);

  
  BookmarkBubbleView(views::View* anchor_view,
                     BookmarkBubbleViewObserver* observer,
                     scoped_ptr<BookmarkBubbleDelegate> delegate,
                     Profile* profile,
                     const GURL& url,
                     bool newly_bookmarked);

  
  base::string16 GetTitle();

  
  virtual gfx::Size GetMinimumSize() OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void OnSelectedIndexChanged(views::Combobox* combobox) OVERRIDE;

  
  void HandleButtonPressed(views::Button* sender);

  
  void ShowEditor();

  
  void ApplyEdits();

  
  static BookmarkBubbleView* bookmark_bubble_;

  
  BookmarkBubbleViewObserver* observer_;

  
  scoped_ptr<BookmarkBubbleDelegate> delegate_;

  
  Profile* profile_;

  
  const GURL url_;

  
  const bool newly_bookmarked_;

  RecentlyUsedFoldersComboModel parent_model_;

  
  views::LabelButton* remove_button_;

  
  views::LabelButton* edit_button_;

  
  views::LabelButton* close_button_;

  
  views::Textfield* title_tf_;

  
  
  views::Combobox* parent_combobox_;

  
  views::View* sync_promo_view_;

  
  bool remove_bookmark_;

  
  bool apply_edits_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkBubbleView);
};

#endif  
