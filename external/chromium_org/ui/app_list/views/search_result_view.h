// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_SEARCH_RESULT_VIEW_H_
#define UI_APP_LIST_VIEWS_SEARCH_RESULT_VIEW_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/app_list/search_result_observer.h"
#include "ui/app_list/views/search_result_actions_view_delegate.h"
#include "ui/views/context_menu_controller.h"
#include "ui/views/controls/button/custom_button.h"

namespace gfx {
class RenderText;
}

namespace views {
class ImageButton;
class ImageView;
class MenuRunner;
}

namespace app_list {

class ProgressBarView;
class SearchResult;
class SearchResultListView;
class SearchResultViewDelegate;
class SearchResultActionsView;

class SearchResultView : public views::CustomButton,
                         public views::ButtonListener,
                         public views::ContextMenuController,
                         public SearchResultObserver,
                         public SearchResultActionsViewDelegate {
 public:
  
  static const char kViewClassName[];

  SearchResultView(SearchResultListView* list_view,
                   SearchResultViewDelegate* delegate);
  virtual ~SearchResultView();

  
  void SetResult(SearchResult* result);
  SearchResult* result() { return result_; }

  
  void ClearResultNoRepaint();

  
  void ClearSelectedAction();

 private:
  void UpdateTitleText();
  void UpdateDetailsText();

  
  virtual const char* GetClassName() const OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual void ChildPreferredSizeChanged(views::View* child) OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void ShowContextMenuForView(views::View* source,
                                      const gfx::Point& point,
                                      ui::MenuSourceType source_type) OVERRIDE;

  
  virtual void OnIconChanged() OVERRIDE;
  virtual void OnActionsChanged() OVERRIDE;
  virtual void OnIsInstallingChanged() OVERRIDE;
  virtual void OnPercentDownloadedChanged() OVERRIDE;
  virtual void OnItemInstalled() OVERRIDE;
  virtual void OnItemUninstalled() OVERRIDE;

  
  virtual void OnSearchResultActionActivated(size_t index,
                                             int event_flags) OVERRIDE;

  SearchResult* result_;  

  
  SearchResultListView* list_view_;

  
  SearchResultViewDelegate* delegate_;

  views::ImageView* icon_;  
  scoped_ptr<gfx::RenderText> title_text_;
  scoped_ptr<gfx::RenderText> details_text_;
  SearchResultActionsView* actions_view_;  
  ProgressBarView* progress_bar_;  

  scoped_ptr<views::MenuRunner> context_menu_runner_;

  DISALLOW_COPY_AND_ASSIGN(SearchResultView);
};

}  

#endif  
