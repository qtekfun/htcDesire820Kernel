// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_INFOBARS_INFOBAR_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_INFOBARS_INFOBAR_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/infobars/infobar.h"
#include "chrome/browser/infobars/infobar_container.h"
#include "third_party/skia/include/core/SkPath.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/menu/menu_item_view.h"
#include "ui/views/focus/external_focus_tracker.h"

namespace ui {
class MenuModel;
}

namespace views {
class ImageButton;
class ImageView;
class Label;
class LabelButton;
class Link;
class LinkListener;
class MenuButton;
class MenuButtonListener;
class MenuRunner;
}  

class InfoBarView : public InfoBar,
                    public views::View,
                    public views::ButtonListener,
                    public views::ExternalFocusTracker {
 public:
  explicit InfoBarView(scoped_ptr<InfoBarDelegate> delegate);

  const SkPath& fill_path() const { return fill_path_; }
  const SkPath& stroke_path() const { return stroke_path_; }

 protected:
  static const int kButtonButtonSpacing;
  static const int kEndOfLabelSpacing;

  virtual ~InfoBarView();

  
  views::Label* CreateLabel(const base::string16& text) const;

  
  
  views::Link* CreateLink(const base::string16& text,
                          views::LinkListener* listener) const;

  
  
  static views::MenuButton* CreateMenuButton(
      const base::string16& text,
      views::MenuButtonListener* menu_button_listener);

  
  
  static views::LabelButton* CreateLabelButton(views::ButtonListener* listener,
                                               const base::string16& text,
                                               bool needs_elevation);

  
  virtual void Layout() OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;

  
  
  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  
  
  virtual int ContentMinimumWidth() const;

  
  
  int StartX() const;
  int EndX() const;

  
  
  
  int OffsetY(const gfx::Size& prefsize) const;

  
  const InfoBarContainer::Delegate* container_delegate() const;

  
  
  
  void RunMenuAt(ui::MenuModel* menu_model,
                 views::MenuButton* button,
                 views::MenuItemView::AnchorPosition anchor);

 private:
  static const int kHorizontalPadding;
  static const int kCloseButtonSpacing;

  
  virtual void PlatformSpecificShow(bool animate) OVERRIDE;
  virtual void PlatformSpecificHide(bool animate) OVERRIDE;
  virtual void PlatformSpecificOnHeightsRecalculated() OVERRIDE;

  
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void PaintChildren(gfx::Canvas* canvas) OVERRIDE;

  
  virtual void OnWillChangeFocus(View* focused_before,
                                 View* focused_now) OVERRIDE;

  
  views::ImageView* icon_;

  
  views::ImageButton* close_button_;

  
  
  SkPath fill_path_;
  SkPath stroke_path_;

  
  scoped_ptr<views::MenuRunner> menu_runner_;

  DISALLOW_COPY_AND_ASSIGN(InfoBarView);
};

#endif  
