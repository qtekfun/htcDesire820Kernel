// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FIND_BAR_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_FIND_BAR_VIEW_H_
#pragma once

#include "base/string16.h"
#include "chrome/browser/ui/find_bar/find_notification_details.h"
#include "chrome/browser/ui/views/dropdown_bar_view.h"
#include "ui/gfx/size.h"
#include "views/controls/button/button.h"
#include "views/controls/textfield/textfield.h"
#include "views/controls/textfield/textfield_controller.h"

class FindBarHost;

namespace views {
class ImageButton;
class Label;
class MouseEvent;
class View;
}

class FindBarView : public DropdownBarView,
                    public views::ButtonListener,
                    public views::TextfieldController {
 public:
  
  enum ButtonTag {
    FIND_PREVIOUS_TAG = 0,  
    FIND_NEXT_TAG,          
    CLOSE_TAG,              
  };

  explicit FindBarView(FindBarHost* host);
  virtual ~FindBarView();

  
  string16 GetFindText() const;
  void SetFindText(const string16& find_text);

  
  string16 GetFindSelectedText() const;

  
  string16 GetMatchCountText() const;

  
  
  void UpdateForResult(const FindNotificationDetails& result,
                       const string16& find_text);

  
  void ClearMatchCount();

  
  virtual void SetFocusAndSelection(bool select_all);

  
  virtual void OnPaint(gfx::Canvas* canvas);
  virtual void Layout();
  virtual gfx::Size GetPreferredSize();
  virtual void ViewHierarchyChanged(bool is_add,
                                    views::View* parent,
                                    views::View* child);

  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event);

  
  virtual void ContentsChanged(views::Textfield* sender,
                               const string16& new_contents);
  virtual bool HandleKeyEvent(views::Textfield* sender,
                              const views::KeyEvent& key_event);

 private:
  
  void UpdateMatchCountAppearance(bool no_match);

  
  virtual void OnThemeChanged();

  
  
  
  
  
  
  class FocusForwarderView : public views::View {
   public:
    explicit FocusForwarderView(
        views::Textfield* view_to_focus_on_mousedown)
      : view_to_focus_on_mousedown_(view_to_focus_on_mousedown) {}

   private:
    virtual bool OnMousePressed(const views::MouseEvent& event);

    views::Textfield* view_to_focus_on_mousedown_;

    DISALLOW_COPY_AND_ASSIGN(FocusForwarderView);
  };

  
  
  class SearchTextfieldView : public views::Textfield {
   public:
     SearchTextfieldView();
     virtual ~SearchTextfieldView();

     virtual void RequestFocus();

   private:
     DISALLOW_COPY_AND_ASSIGN(SearchTextfieldView);
  };

  
  
  FindBarHost* find_bar_host() const;

  
  SearchTextfieldView* find_text_;
  views::Label* match_count_text_;
  FocusForwarderView* focus_forwarder_view_;
  views::ImageButton* find_previous_button_;
  views::ImageButton* find_next_button_;
  views::ImageButton* close_button_;

  DISALLOW_COPY_AND_ASSIGN(FindBarView);
};

#endif  
