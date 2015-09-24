// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_BOUNDED_LABEL_H_
#define UI_MESSAGE_CENTER_BOUNDED_LABEL_H_

#include <list>
#include <map>

#include "base/memory/scoped_ptr.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/message_center/message_center_export.h"
#include "ui/views/view.h"

namespace gfx {
class FontList;
}

namespace message_center {

class InnerBoundedLabel;

namespace test {
class BoundedLabelTest;
}

class MESSAGE_CENTER_EXPORT BoundedLabel : public views::View {
 public:
  BoundedLabel(const string16& text, const gfx::FontList& font_list);
  BoundedLabel(const string16& text);
  virtual ~BoundedLabel();

  void SetColors(SkColor textColor, SkColor backgroundColor);
  void SetLineHeight(int height);  
  void SetLineLimit(int lines);  

  int GetLineHeight() const;
  int GetLineLimit() const;

  
  int GetLinesForWidthAndLimit(int width, int limit);
  gfx::Size GetSizeForWidthAndLines(int width, int lines);

  
  virtual int GetBaseline() const OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual int GetHeightForWidth(int width) OVERRIDE;
  virtual void Paint(gfx::Canvas* canvas) OVERRIDE;
  virtual bool HitTestRect(const gfx::Rect& rect) const OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

 protected:
  
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;
  virtual void OnNativeThemeChanged(const ui::NativeTheme* theme) OVERRIDE;

 private:
  friend class test::BoundedLabelTest;

  string16 GetWrappedTextForTest(int width, int lines);

  scoped_ptr<InnerBoundedLabel> label_;
  int line_limit_;

  DISALLOW_COPY_AND_ASSIGN(BoundedLabel);
};

}  

#endif  
