// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_TRAY_TRAY_DETAILS_VIEW_H_
#define ASH_SYSTEM_TRAY_TRAY_DETAILS_VIEW_H_

#include "ash/ash_export.h"
#include "ash/system/tray/special_popup_row.h"
#include "ui/views/view.h"

namespace views {
class ScrollView;
}

namespace ash {

class SystemTrayItem;

namespace internal {

class FixedSizedScrollView;
class ScrollBorder;
class ViewClickListener;

class ASH_EXPORT TrayDetailsView : public views::View {
 public:
  explicit TrayDetailsView(SystemTrayItem* owner);
  virtual ~TrayDetailsView();

  
  
  void CreateSpecialRow(int string_id, ViewClickListener* listener);

  
  
  void CreateScrollableList();

  
  void AddScrollSeparator();

  
  void Reset();

  
  
  
  void TransitionToDefaultView();

  SystemTrayItem* owner() const { return owner_; }
  SpecialPopupRow* footer() const { return footer_; }
  FixedSizedScrollView* scroller() const { return scroller_; }
  views::View* scroll_content() const { return scroll_content_; }

 protected:
  
  virtual void Layout() OVERRIDE;
  virtual void OnPaintBorder(gfx::Canvas* canvas) OVERRIDE;

 private:
  SystemTrayItem* owner_;
  SpecialPopupRow* footer_;
  FixedSizedScrollView* scroller_;
  views::View* scroll_content_;
  ScrollBorder* scroll_border_;

  DISALLOW_COPY_AND_ASSIGN(TrayDetailsView);
};

}  
}  

#endif  
