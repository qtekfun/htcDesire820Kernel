// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_APP_LIST_MAIN_VIEW_H_
#define UI_APP_LIST_VIEWS_APP_LIST_MAIN_VIEW_H_

#include <string>

#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/timer/timer.h"
#include "ui/app_list/app_list_export.h"
#include "ui/app_list/views/apps_grid_view_delegate.h"
#include "ui/app_list/views/search_box_view_delegate.h"
#include "ui/app_list/views/search_result_list_view_delegate.h"
#include "ui/views/view.h"

namespace views {
class Widget;
}

namespace app_list {

class ApplicationDragAndDropHost;
class AppListModel;
class AppListItemModel;
class AppListViewDelegate;
class ContentsView;
class PaginationModel;
class SearchBoxView;

class APP_LIST_EXPORT AppListMainView : public views::View,
                                        public AppsGridViewDelegate,
                                        public SearchBoxViewDelegate,
                                        public SearchResultListViewDelegate {
 public:
  
  explicit AppListMainView(AppListViewDelegate* delegate,
                           PaginationModel* pagination_model,
                           gfx::NativeView parent);
  virtual ~AppListMainView();

  void ShowAppListWhenReady();

  void Close();

  void Prerender();

  void ModelChanged();

  SearchBoxView* search_box_view() const { return search_box_view_; }

  
  
  void SetDragAndDropHostOfCurrentAppList(
      ApplicationDragAndDropHost* drag_and_drop_host);

  ContentsView* contents_view() { return contents_view_; }

 private:
  class IconLoader;

  void AddContentsView();

  
  
  void PreloadIcons(gfx::NativeView parent);

  
  void OnIconLoadingWaitTimer();

  
  void OnItemIconLoaded(IconLoader* loader);

  
  virtual void ActivateApp(AppListItemModel* item, int event_flags) OVERRIDE;
  virtual void GetShortcutPathForApp(
      const std::string& app_id,
      const base::Callback<void(const base::FilePath&)>& callback) OVERRIDE;

  
  virtual void QueryChanged(SearchBoxView* sender) OVERRIDE;

  
  virtual void OpenResult(SearchResult* result,
                          int event_flags) OVERRIDE;
  virtual void InvokeResultAction(SearchResult* result,
                                  int action_index,
                                  int event_flags) OVERRIDE;
  virtual void OnResultInstalled(SearchResult* result) OVERRIDE;
  virtual void OnResultUninstalled(SearchResult* result) OVERRIDE;

  AppListViewDelegate* delegate_;  
  PaginationModel* pagination_model_;  
  AppListModel* model_;  

  SearchBoxView* search_box_view_;  
  ContentsView* contents_view_;  

  
  
  base::OneShotTimer<AppListMainView> icon_loading_wait_timer_;

  ScopedVector<IconLoader> pending_icon_loaders_;

  base::WeakPtrFactory<AppListMainView> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(AppListMainView);
};

}  

#endif  
