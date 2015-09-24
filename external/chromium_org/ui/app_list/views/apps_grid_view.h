// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_APPS_GRID_VIEW_H_
#define UI_APP_LIST_VIEWS_APPS_GRID_VIEW_H_

#include <set>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/timer/timer.h"
#include "ui/app_list/app_list_export.h"
#include "ui/app_list/app_list_model.h"
#include "ui/app_list/app_list_model_observer.h"
#include "ui/app_list/pagination_model_observer.h"
#include "ui/base/models/list_model_observer.h"
#include "ui/views/animation/bounds_animator.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/view.h"
#include "ui/views/view_model.h"

#if defined(OS_WIN)
#include "ui/base/dragdrop/drag_source_win.h"
#endif

namespace content {
class WebContents;
}

namespace views {
class ButtonListener;
class DragImageView;
class WebView;
}

namespace app_list {

#if defined(OS_WIN)
class SynchronousDrag;
#endif

namespace test {
class AppsGridViewTestApi;
}

class ApplicationDragAndDropHost;
class AppListItemView;
class AppsGridViewDelegate;
class PageSwitcher;
class PaginationModel;

class APP_LIST_EXPORT AppsGridView : public views::View,
                                     public views::ButtonListener,
                                     public AppListItemListObserver,
                                     public PaginationModelObserver,
                                     public AppListModelObserver {
 public:
  enum Pointer {
    NONE,
    MOUSE,
    TOUCH,
  };

  
  
  
  
  
  AppsGridView(AppsGridViewDelegate* delegate,
               PaginationModel* pagination_model,
               content::WebContents* start_page_contents);
  virtual ~AppsGridView();

  
  
  void SetLayout(int icon_size, int cols, int rows_per_page);

  
  void SetModel(AppListModel* model);

  
  
  void SetItemList(AppListItemList* item_list);

  void SetSelectedView(views::View* view);
  void ClearSelectedView(views::View* view);
  void ClearAnySelectedView();
  bool IsSelectedView(const views::View* view) const;

  
  
  void EnsureViewVisible(const views::View* view);

  void InitiateDrag(AppListItemView* view,
                    Pointer pointer,
                    const ui::LocatedEvent& event);

  
  void UpdateDragFromItem(Pointer pointer,
                          const ui::LocatedEvent& event);

  
  
  void UpdateDrag(Pointer pointer, const gfx::Point& point);
  void EndDrag(bool cancel);
  bool IsDraggedView(const views::View* view) const;

  void StartSettingUpSynchronousDrag();
  bool RunSynchronousDrag();
  void CleanUpSynchronousDrag();
  void OnGotShortcutPath(const base::FilePath& path);

  
  void SetDragAndDropHostOfCurrentAppList(
      ApplicationDragAndDropHost* drag_and_drop_host);

  
  void Prerender(int page_index);

  bool has_dragged_view() const { return drag_view_ != NULL; }
  bool dragging() const { return drag_pointer_ != NONE; }

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual bool OnKeyReleased(const ui::KeyEvent& event) OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;
  virtual bool GetDropFormats(
      int* formats,
      std::set<OSExchangeData::CustomFormat>* custom_formats) OVERRIDE;
  virtual bool CanDrop(const OSExchangeData& data) OVERRIDE;
  virtual int OnDragUpdated(const ui::DropTargetEvent& event) OVERRIDE;

  
  void StopPageFlipTimer();

  
  const views::ViewModel* view_model_for_test() const { return &view_model_; }

  
  bool has_drag_and_drop_host_for_test() { return NULL != drag_and_drop_host_; }

  
  bool forward_events_to_drag_and_drop_host_for_test() {
    return forward_events_to_drag_and_drop_host_;
  }

  void set_is_root_level(bool value) { is_root_level_ = value; }

 private:
  friend class test::AppsGridViewTestApi;

  enum DropAttempt {
    DROP_FOR_NONE,
    DROP_FOR_REORDER,
    DROP_FOR_FOLDER,
  };

  
  struct Index {
    Index() : page(-1), slot(-1) {}
    Index(int page, int slot) : page(page), slot(slot) {}

    bool operator==(const Index& other) const {
      return page == other.page && slot == other.slot;
    }
    bool operator!=(const Index& other) const {
      return page != other.page || slot != other.slot;
    }

    int page;  
    int slot;  
  };

  int tiles_per_page() const { return cols_ * rows_per_page_; }

  
  void Update();

  
  void UpdatePaging();

  
  
  void UpdatePulsingBlockViews();

  views::View* CreateViewForItemAtIndex(size_t index);

  
  
  
  Index GetIndexFromModelIndex(int model_index) const;
  int GetModelIndexFromIndex(const Index& index) const;

  void SetSelectedItemByIndex(const Index& index);
  bool IsValidIndex(const Index& index) const;

  Index GetIndexOfView(const views::View* view) const;
  views::View* GetViewAtIndex(const Index& index) const;

  void MoveSelected(int page_delta, int slot_x_delta, int slot_y_delta);

  void CalculateIdealBounds();
  void AnimateToIdealBounds();

  
  
  
  
  
  
  
  void AnimationBetweenRows(views::View* view,
                            bool animate_current,
                            const gfx::Rect& current,
                            bool animate_target,
                            const gfx::Rect& target);

  
  void ExtractDragLocation(const ui::LocatedEvent& event,
                           gfx::Point* drag_point);

  
  
  
  
  void CalculateDropTarget(const gfx::Point& drag_point,
                           bool use_page_button_hovering);

  
  
  
  
  void CalculateDropTargetWithFolderEnabled(const gfx::Point& drag_point,
                                            bool use_page_button_hovering);

  
  
  void StartDragAndDropHostDrag(const gfx::Point& grid_location);

  
  void DispatchDragEventToDragAndDropHost(
      const gfx::Point& location_in_screen_coordinates);

  
  
  void MaybeStartPageFlipTimer(const gfx::Point& drag_point);

  
  void OnPageFlipTimer();

  
  void MoveItemInModel(views::View* item_view, const Index& target);

  
  
  
  void MoveItemToFolder(views::View* item_view, const Index& target);

  
  void CancelContextMenusOnCurrentPage();

  
  
  bool IsPointWithinDragBuffer(const gfx::Point& point) const;

  
  void LayoutStartPage();

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void OnListItemAdded(size_t index, AppListItemModel* item) OVERRIDE;
  virtual void OnListItemRemoved(size_t index, AppListItemModel* item) OVERRIDE;
  virtual void OnListItemMoved(size_t from_index,
                               size_t to_index,
                               AppListItemModel* item) OVERRIDE;

  
  virtual void TotalPagesChanged() OVERRIDE;
  virtual void SelectedPageChanged(int old_selected, int new_selected) OVERRIDE;
  virtual void TransitionStarted() OVERRIDE;
  virtual void TransitionChanged() OVERRIDE;

  
  virtual void OnAppListModelStatusChanged() OVERRIDE;

  
  
  
  
  void SetViewHidden(views::View* view, bool hide, bool immediate);

  
  bool EnableFolderDragDropUI();

  
  
  bool CanDropIntoTarget(const Index& drop_target);

  
  
  Index GetNearestTileForDragView();

  
  
  
  
  void CalculateNearestTileForVertex(
      const gfx::Point& vertex, Index* nearest_tile, int* d_min);

  
  
  gfx::Rect GetTileBoundsForPoint(const gfx::Point& point, Index* tile_index);

  
  gfx::Rect GetTileBounds(int row, int col) const;

  
  
  views::View* GetViewAtSlotOnCurrentPage(int slot);

  
  
  void SetAsFolderDroppingTarget(const Index& target_index,
                                 bool is_target_folder);

  
  void OnReorderTimer();

  
  
  void OnFolderDroppingTimer();

  AppListModel* model_;  
  AppListItemList* item_list_;  
  AppsGridViewDelegate* delegate_;
  PaginationModel* pagination_model_;  
  PageSwitcher* page_switcher_view_;  
  views::WebView* start_page_view_;  

  gfx::Size icon_size_;
  int cols_;
  int rows_per_page_;

  
  views::ViewModel view_model_;

  
  views::ViewModel pulsing_blocks_model_;

  views::View* selected_view_;

  AppListItemView* drag_view_;

  
  gfx::Point drag_view_offset_;

  
  gfx::Point drag_start_grid_view_;

  
  gfx::Point drag_view_start_;

  
  int drag_start_page_;

#if defined(OS_WIN)
  
  
  scoped_refptr<SynchronousDrag> synchronous_drag_;
#endif

  Pointer drag_pointer_;
  Index drop_target_;
  DropAttempt drop_attempt_;

  
  base::OneShotTimer<AppsGridView> reorder_timer_;

  
  
  base::OneShotTimer<AppsGridView> folder_dropping_timer_;

  
  ApplicationDragAndDropHost* drag_and_drop_host_;

  
  
  bool forward_events_to_drag_and_drop_host_;

  
  gfx::Point last_drag_point_;

  
  base::OneShotTimer<AppsGridView> page_flip_timer_;

  
  int page_flip_target_;

  
  
  int page_flip_delay_in_ms_;

  views::BoundsAnimator bounds_animator_;

  
  bool is_root_level_;

  DISALLOW_COPY_AND_ASSIGN(AppsGridView);
};

}  

#endif  
