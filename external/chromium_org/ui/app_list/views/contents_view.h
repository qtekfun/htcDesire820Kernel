// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_CONTENTS_VIEW_H_
#define UI_APP_LIST_VIEWS_CONTENTS_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/views/view.h"

namespace content {
class WebContents;
}

namespace views {
class BoundsAnimator;
class ViewModel;
}

namespace app_list {

class AppsGridView;
class ApplicationDragAndDropHost;
class AppListFolderItem;
class AppListMainView;
class AppListModel;
class AppListViewDelegate;
class AppsContainerView;
class PaginationModel;

class ContentsView : public views::View {
 public:
  ContentsView(AppListMainView* app_list_main_view,
               PaginationModel* pagination_model,
               AppListModel* model,
               content::WebContents* start_page_contents);
  virtual ~ContentsView();

  
  void CancelDrag();

  
  
  void SetDragAndDropHostOfCurrentAppList(
      ApplicationDragAndDropHost* drag_and_drop_host);

  void ShowSearchResults(bool show);
  void ShowFolderContent(AppListFolderItem* folder);

  void Prerender();

  AppsContainerView* apps_container_view() { return apps_container_view_; }

 private:
  enum ShowState {
    SHOW_APPS,
    SHOW_SEARCH_RESULTS,
  };

  
  void SetShowState(ShowState show_state);

  
  void ShowStateChanged();

  void CalculateIdealBounds();
  void AnimateToIdealBounds();

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual bool OnMouseWheel(const ui::MouseWheelEvent& event) OVERRIDE;

  
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual void OnScrollEvent(ui::ScrollEvent* event) OVERRIDE;

  ShowState show_state_;
  PaginationModel* pagination_model_;  

  AppsContainerView* apps_container_view_;  

  scoped_ptr<views::ViewModel> view_model_;
  scoped_ptr<views::BoundsAnimator> bounds_animator_;

  DISALLOW_COPY_AND_ASSIGN(ContentsView);
};

}  

#endif  
