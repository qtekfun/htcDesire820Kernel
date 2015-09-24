// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_SYNC_ONE_CLICK_SIGNIN_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_SYNC_ONE_CLICK_SIGNIN_BUBBLE_VIEW_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/sync/one_click_signin_bubble_delegate.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/link_listener.h"

namespace base {
class MessageLoop;
}

namespace views {
class GridLayout;
class ImageButton;
class LabelButton;
class View;
}

class OneClickSigninBubbleView : public views::BubbleDelegateView,
                                 public views::LinkListener,
                                 public views::ButtonListener {
 public:
  
  
  
  static void ShowBubble(BrowserWindow::OneClickSigninBubbleType type,
                         const base::string16& email,
                         const base::string16& error_message,
                         scoped_ptr<OneClickSigninBubbleDelegate> delegate,
                         views::View* anchor_view,
                         const BrowserWindow::StartSyncCallback& start_sync);

  static bool IsShowing();

  static void Hide();

  
  
  static OneClickSigninBubbleView* view_for_testing() { return bubble_view_; }

 protected:
  
  OneClickSigninBubbleView(
      const base::string16& error_message,
      const base::string16& email,
      scoped_ptr<OneClickSigninBubbleDelegate> delegate,
      views::View* anchor_view,
      const BrowserWindow::StartSyncCallback& start_sync_callback,
      bool is_sync_dialog);

  virtual ~OneClickSigninBubbleView();

 private:
  friend class OneClickSigninBubbleViewTest;

  FRIEND_TEST_ALL_PREFIXES(OneClickSigninBubbleViewTest, BubbleOkButton);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninBubbleViewTest, DialogOkButton);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninBubbleViewTest, DialogUndoButton);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninBubbleViewTest, BubbleAdvancedLink);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninBubbleViewTest, DialogAdvancedLink);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninBubbleViewTest, BubbleLearnMoreLink);
  FRIEND_TEST_ALL_PREFIXES(OneClickSigninBubbleViewTest, DialogLearnMoreLink);

  
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;
  virtual void Init() OVERRIDE;

  
  virtual void LinkClicked(views::Link* source, int event_flags) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;

  
  virtual void WindowClosing() OVERRIDE;
  virtual ui::ModalType GetModalType() const OVERRIDE;

  
  void InitBubbleContent(views::GridLayout* layout);

  
  void InitDialogContent(views::GridLayout* layout);

  
  void InitButtons(views::GridLayout* layout);
  void GetButtons(views::LabelButton** ok_button,
                          views::LabelButton** undo_button);

  
  void InitLearnMoreLink();

  
  void InitAdvancedLink();

  
  scoped_ptr<OneClickSigninBubbleDelegate> delegate_;

  
  const base::string16 error_message_;

  
  const base::string16 email_;

  
  
  
  BrowserWindow::StartSyncCallback start_sync_callback_;

  const bool is_sync_dialog_;

  
  views::Link* advanced_link_;

  
  views::Link* learn_more_link_;

  
  views::LabelButton* ok_button_;
  views::LabelButton* undo_button_;

  
  views::ImageButton* close_button_;

  bool clicked_learn_more_;

  
  base::MessageLoop* message_loop_for_testing_;

  
  static OneClickSigninBubbleView* bubble_view_;

  DISALLOW_COPY_AND_ASSIGN(OneClickSigninBubbleView);
};

#endif  
