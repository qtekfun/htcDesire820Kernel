// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_TRAY_SPECIAL_POPUP_ROW_H_
#define ASH_SYSTEM_TRAY_SPECIAL_POPUP_ROW_H_

#include "ui/gfx/size.h"
#include "ui/views/view.h"

namespace views {
class Label;
}

namespace ash {
namespace internal {

class ThrobberView;
class TrayItemView;
class TrayPopupHeaderButton;
class ViewClickListener;

class SpecialPopupRow : public views::View {
 public:
  SpecialPopupRow();
  virtual ~SpecialPopupRow();

  void SetTextLabel(int string_id, ViewClickListener* listener);
  void SetContent(views::View* view);

  void AddButton(TrayPopupHeaderButton* button);
  void AddThrobber(ThrobberView* throbber);

  views::View* content() const { return content_; }

 private:
  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual int GetHeightForWidth(int width) OVERRIDE;
  virtual void Layout() OVERRIDE;

  views::View* content_;
  views::View* button_container_;
  views::Label* text_label_;

  DISALLOW_COPY_AND_ASSIGN(SpecialPopupRow);
};

}  
}  

#endif  
