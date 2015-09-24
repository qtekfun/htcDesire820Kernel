// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_INFOBARS_INFOBAR_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_INFOBARS_INFOBAR_VIEW_H_
#pragma once

#include "base/task.h"
#include "chrome/browser/ui/views/infobars/infobar.h"
#include "chrome/browser/ui/views/infobars/infobar_background.h"
#include "chrome/browser/ui/views/infobars/infobar_container.h"
#include "views/controls/button/button.h"
#include "views/focus/focus_manager.h"

class SkPath;

namespace views {
class ExternalFocusTracker;
class ImageButton;
class ImageView;
class Label;
class Link;
class LinkController;
class MenuButton;
class TextButton;
class ViewMenuDelegate;
}

class InfoBarView : public InfoBar,
                    public views::View,
                    public views::ButtonListener,
                    public views::FocusChangeListener {
 public:
  explicit InfoBarView(InfoBarDelegate* delegate);

  SkPath* fill_path() const { return fill_path_.get(); }
  SkPath* stroke_path() const { return stroke_path_.get(); }

 protected:
  static const int kButtonButtonSpacing;
  static const int kEndOfLabelSpacing;

  virtual ~InfoBarView();

  
  static views::Label* CreateLabel(const string16& text);

  
  static views::Link* CreateLink(const string16& text,
                                 views::LinkController* controller,
                                 const SkColor& background_color);

  
  static views::MenuButton* CreateMenuButton(
      const string16& text,
      bool normal_has_border,
      views::ViewMenuDelegate* menu_delegate);

  
  static views::TextButton* CreateTextButton(views::ButtonListener* listener,
                                             const string16& text,
                                             bool needs_elevation);

  
  virtual void Layout() OVERRIDE;
  virtual void ViewHierarchyChanged(bool is_add,
                                    View* parent,
                                    View* child) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const views::Event& event) OVERRIDE;

  
  
  
  virtual int ContentMinimumWidth() const;

  
  
  int StartX() const;
  int EndX() const;

  
  const InfoBarContainer::Delegate* container_delegate() const;

 private:
  static const int kHorizontalPadding;

  
  virtual void PlatformSpecificHide(bool animate) OVERRIDE;
  virtual void PlatformSpecificOnHeightsRecalculated() OVERRIDE;

  
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void PaintChildren(gfx::Canvas* canvas) OVERRIDE;

  
  virtual void FocusWillChange(View* focused_before,
                               View* focused_now) OVERRIDE;

  
  
  
  void DestroyFocusTracker(bool restore_focus);

  
  
  void DeleteSelf();

  
  views::ImageView* icon_;

  
  views::ImageButton* close_button_;

  
  
  scoped_ptr<views::ExternalFocusTracker> focus_tracker_;

  
  ScopedRunnableMethodFactory<InfoBarView> delete_factory_;

  
  
  scoped_ptr<SkPath> fill_path_;
  scoped_ptr<SkPath> stroke_path_;

  DISALLOW_COPY_AND_ASSIGN(InfoBarView);
};

#endif  
