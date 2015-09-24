// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_SCROLLBAR_SCROLL_BAR_H_
#define UI_VIEWS_CONTROLS_SCROLLBAR_SCROLL_BAR_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/view.h"
#include "ui/views/views_export.h"

namespace views {

class ScrollBar;

class VIEWS_EXPORT ScrollBarController {
 public:
  
  
  
  
  
  virtual void ScrollToPosition(ScrollBar* source, int position) = 0;

  
  
  
  
  
  
  
  
  
  virtual int GetScrollIncrement(ScrollBar* source,
                                 bool is_page,
                                 bool is_positive) = 0;
};

class VIEWS_EXPORT ScrollBar : public View {
 public:
  virtual ~ScrollBar();

  
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  bool IsHorizontal() const;

  void set_controller(ScrollBarController* controller) {
     controller_ = controller;
  }
  ScrollBarController* controller() const { return controller_; }

  
  
  virtual void Update(int viewport_size, int content_size, int current_pos);

  
  int GetMaxPosition() const;
  int GetMinPosition() const;

  
  virtual int GetPosition() const = 0;

  
  
  
  virtual int GetLayoutSize() const = 0;

  
  
  virtual int GetContentOverlapSize() const;

  virtual void OnMouseEnteredScrollView(const ui::MouseEvent& event);
  virtual void OnMouseExitedScrollView(const ui::MouseEvent& event);

 protected:
  
  
  
  explicit ScrollBar(bool is_horiz);

 private:
  const bool is_horiz_;

  ScrollBarController* controller_;

  int max_pos_;

  DISALLOW_COPY_AND_ASSIGN(ScrollBar);
};

}  

#endif  
