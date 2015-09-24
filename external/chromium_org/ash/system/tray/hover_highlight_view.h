// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_TRAY_HOVER_HIGHLIGHT_VIEW_H_
#define ASH_SYSTEM_TRAY_HOVER_HIGHLIGHT_VIEW_H_

#include "ash/system/tray/actionable_view.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/gfx/font.h"

namespace views {
class Label;
}

namespace ash {
namespace internal {

class ViewClickListener;

class HoverHighlightView : public ActionableView {
 public:
  explicit HoverHighlightView(ViewClickListener* listener);
  virtual ~HoverHighlightView();

  
  
  void AddIconAndLabel(const gfx::ImageSkia& image,
                       const base::string16& text,
                       gfx::Font::FontStyle style);

  
  
  
  views::Label* AddLabel(const base::string16& text,
                         gfx::Font::FontStyle style);

  
  
  
  views::Label* AddCheckableLabel(const base::string16& text,
                                  gfx::Font::FontStyle style,
                                  bool checked);

  
  
  void SetExpandable(bool expandable);

  void set_highlight_color(SkColor color) { highlight_color_ = color; }
  void set_default_color(SkColor color) { default_color_ = color; }
  void set_text_highlight_color(SkColor c) { text_highlight_color_ = c; }
  void set_text_default_color(SkColor color) { text_default_color_ = color; }

  views::Label* text_label() { return text_label_; }

  bool hover() const { return hover_; }

 private:
  
  virtual bool PerformAction(const ui::Event& event) OVERRIDE;

  
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual int GetHeightForWidth(int width) OVERRIDE;
  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnEnabledChanged() OVERRIDE;
  virtual void OnPaintBackground(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnFocus() OVERRIDE;

  ViewClickListener* listener_;
  views::Label* text_label_;
  SkColor highlight_color_;
  SkColor default_color_;
  SkColor text_highlight_color_;
  SkColor text_default_color_;
  bool hover_;
  bool expandable_;
  bool checkable_;
  bool checked_;

  DISALLOW_COPY_AND_ASSIGN(HoverHighlightView);
};

}  
}  

#endif  
