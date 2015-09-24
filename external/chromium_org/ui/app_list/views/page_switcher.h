// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_PAGE_SWITCHER_H_
#define UI_APP_LIST_VIEWS_PAGE_SWITCHER_H_

#include "base/basictypes.h"
#include "ui/app_list/pagination_model_observer.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/view.h"

namespace app_list {

class PaginationModel;

class PageSwitcher : public views::View,
                     public views::ButtonListener,
                     public PaginationModelObserver {
 public:
  explicit PageSwitcher(PaginationModel* model);
  virtual ~PageSwitcher();

  
  
  
  int GetPageForPoint(const gfx::Point& point) const;

  
  
  void UpdateUIForDragPoint(const gfx::Point& point);

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;

 private:
  void CalculateButtonWidthAndSpacing(int contents_width);

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void TotalPagesChanged() OVERRIDE;
  virtual void SelectedPageChanged(int old_selected, int new_selected) OVERRIDE;
  virtual void TransitionStarted() OVERRIDE;
  virtual void TransitionChanged() OVERRIDE;

  PaginationModel* model_;  
  views::View* buttons_;  

  DISALLOW_COPY_AND_ASSIGN(PageSwitcher);
};

}  

#endif  
