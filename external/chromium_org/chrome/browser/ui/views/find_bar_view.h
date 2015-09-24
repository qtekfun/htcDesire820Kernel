// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FIND_BAR_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_FIND_BAR_VIEW_H_

#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "chrome/browser/ui/views/dropdown_bar_view.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/textfield/textfield.h"
#include "ui/views/controls/textfield/textfield_controller.h"

class FindBarHost;
class FindNotificationDetails;

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

  
  void SetFindTextAndSelectedRange(const base::string16& find_text,
                                   const gfx::Range& selected_range);
  base::string16 GetFindText() const;
  gfx::Range GetSelectedRange() const;

  
  base::string16 GetFindSelectedText() const;

  
  base::string16 GetMatchCountText() const;

  
  
  void UpdateForResult(const FindNotificationDetails& result,
                       const base::string16& find_text);

  
  void ClearMatchCount();

  
  virtual void SetFocusAndSelection(bool select_all) OVERRIDE;

  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual bool HandleKeyEvent(views::Textfield* sender,
                              const ui::KeyEvent& key_event) OVERRIDE;
  virtual void OnAfterUserAction(views::Textfield* sender) OVERRIDE;
  virtual void OnAfterPaste() OVERRIDE;

 private:
  
  void Find(const base::string16& search_text);

  
  void UpdateMatchCountAppearance(bool no_match);

  
  virtual void OnThemeChanged() OVERRIDE;

  
  
  
  
  
  
  class FocusForwarderView : public views::View {
   public:
    explicit FocusForwarderView(
        views::Textfield* view_to_focus_on_mousedown)
      : view_to_focus_on_mousedown_(view_to_focus_on_mousedown) {}

   private:
    virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;

    views::Textfield* view_to_focus_on_mousedown_;

    DISALLOW_COPY_AND_ASSIGN(FocusForwarderView);
  };

  
  
  FindBarHost* find_bar_host() const;

  
  
  base::string16 last_searched_text_;

  
  views::Textfield* find_text_;
  views::Label* match_count_text_;
  FocusForwarderView* focus_forwarder_view_;
  views::ImageButton* find_previous_button_;
  views::ImageButton* find_next_button_;
  views::ImageButton* close_button_;

  
  int preferred_height_;

  
  
  const gfx::ImageSkia* text_box_background_;

  
  const gfx::ImageSkia* text_box_background_left_;

  DISALLOW_COPY_AND_ASSIGN(FindBarView);
};

#endif  
