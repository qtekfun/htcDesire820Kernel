// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELF_SHELF_VIEW_H_
#define ASH_SHELF_SHELF_VIEW_H_

#include <utility>
#include <vector>

#include "ash/shelf/shelf_button_host.h"
#include "ash/shelf/shelf_model_observer.h"
#include "ash/wm/gestures/shelf_gesture_handler.h"
#include "base/observer_list.h"
#include "ui/app_list/views/app_list_drag_and_drop_host.h"
#include "ui/views/animation/bounds_animator_observer.h"
#include "ui/views/context_menu_controller.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/focus/focus_manager.h"
#include "ui/views/view.h"

namespace views {
class BoundsAnimator;
class MenuModelAdapter;
class MenuRunner;
class ViewModel;
}

namespace ash {

namespace test {
class ShelfViewTestAPI;
}

class ShelfDelegate;
class ShelfIconObserver;
class ShelfItemDelegateManager;
class ShelfModel;
struct LauncherItem;

namespace internal {

class DragImageView;
class OverflowBubble;
class OverflowButton;
class ShelfButton;
class ShelfLayoutManager;
class ShelfTooltipManager;

extern const int SHELF_ALIGNMENT_UMA_ENUM_VALUE_BOTTOM;
extern const int SHELF_ALIGNMENT_UMA_ENUM_VALUE_LEFT;
extern const int SHELF_ALIGNMENT_UMA_ENUM_VALUE_RIGHT;
extern const int SHELF_ALIGNMENT_UMA_ENUM_VALUE_COUNT;

class ASH_EXPORT ShelfView : public views::View,
                             public ShelfModelObserver,
                             public views::ButtonListener,
                             public ShelfButtonHost,
                             public views::ContextMenuController,
                             public views::FocusTraversable,
                             public views::BoundsAnimatorObserver,
                             public app_list::ApplicationDragAndDropHost {
 public:
  ShelfView(ShelfModel* model,
            ShelfDelegate* delegate,
            ShelfLayoutManager* manager);
  virtual ~ShelfView();

  ShelfTooltipManager* tooltip_manager() { return tooltip_.get(); }

  ShelfLayoutManager* shelf_layout_manager() { return layout_manager_; }

  ShelfModel* model() { return model_; }

  void Init();

  void OnShelfAlignmentChanged();
  void SchedulePaintForAllButtons();

  
  
  gfx::Rect GetIdealBoundsOfItemIcon(LauncherID id);

  
  void UpdatePanelIconPosition(LauncherID id, const gfx::Point& midpoint);

  void AddIconObserver(ShelfIconObserver* observer);
  void RemoveIconObserver(ShelfIconObserver* observer);

  
  bool IsShowingMenu() const;

  
  bool IsShowingOverflowBubble() const;

  
  
  void set_owner_overflow_bubble(OverflowBubble* owner) {
    owner_overflow_bubble_ = owner;
  }

  views::View* GetAppListButtonView() const;

  
  
  
  
  bool ShouldHideTooltip(const gfx::Point& cursor_location);

  
  
  gfx::Rect GetVisibleItemsBoundsInScreen();

  
  virtual views::FocusSearch* GetFocusSearch() OVERRIDE;
  virtual FocusTraversable* GetFocusTraversableParent() OVERRIDE;
  virtual View* GetFocusTraversableParentView() OVERRIDE;

  
  virtual void CreateDragIconProxy(
      const gfx::Point& location_in_screen_coordinates,
      const gfx::ImageSkia& icon,
      views::View* replaced_view,
      const gfx::Vector2d& cursor_offset_from_center,
      float scale_factor) OVERRIDE;
  virtual void UpdateDragIconProxy(
      const gfx::Point& location_in_screen_coordinates) OVERRIDE;
  virtual void DestroyDragIconProxy() OVERRIDE;
  virtual bool StartDrag(
      const std::string& app_id,
      const gfx::Point& location_in_screen_coordinates) OVERRIDE;
  virtual bool Drag(const gfx::Point& location_in_screen_coordinates) OVERRIDE;
  virtual void EndDrag(bool cancel) OVERRIDE;

  
  const views::ViewModel* const view_model_for_test() const {
    return view_model_.get();
  }

 private:
  friend class ash::test::ShelfViewTestAPI;

  class FadeOutAnimationDelegate;
  class StartFadeAnimationDelegate;

  struct IdealBounds {
    gfx::Rect overflow_bounds;
  };

  enum RemovableState {
    REMOVABLE,     
    DRAGGABLE,     
    NOT_REMOVABLE, 
  };

  
  
  
  
  
  
  bool is_overflow_mode() const { return overflow_mode_; }

  bool dragging() const {
    return drag_pointer_ != NONE;
  }

  
  void LayoutToIdealBounds();

  
  void UpdateAllButtonsVisibilityInOverflowMode();

  
  
  void CalculateIdealBounds(IdealBounds* bounds);

  
  
  int DetermineLastVisibleIndex(int max_value) const;

  
  
  int DetermineFirstVisiblePanelIndex(int min_value) const;

  
  void AnimateToIdealBounds();

  
  views::View* CreateViewForItem(const LauncherItem& item);

  
  void FadeIn(views::View* view);

  
  
  void PrepareForDrag(Pointer pointer, const ui::LocatedEvent& event);

  
  void ContinueDrag(const ui::LocatedEvent& event);

  
  
  bool HandleRipOffDrag(const ui::LocatedEvent& event);

  
  void FinalizeRipOffDrag(bool cancel);

  
  RemovableState RemovableByRipOff(int index);

  
  bool SameDragType(LauncherItemType typea, LauncherItemType typeb) const;

  
  
  std::pair<int, int> GetDragRange(int index);

  
  
  int CancelDrag(int modified_index);

  
  
  
  
  
  
  
  gfx::Rect GetBoundsForDragInsertInScreen();

  
  void ConfigureChildView(views::View* view);

  
  void ToggleOverflowBubble();

  
  
  
  void UpdateFirstButtonPadding();

  
  void OnFadeOutAnimationEnded();

  
  void StartFadeInLastVisibleItem();

  
  void UpdateOverflowRange(ShelfView* overflow_view);

  
  int GetButtonSize() const;

  
  int GetButtonSpacing() const;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;
  virtual FocusTraversable* GetPaneFocusTraversable() OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  
  virtual void ShelfItemAdded(int model_index) OVERRIDE;
  virtual void ShelfItemRemoved(int model_index, LauncherID id) OVERRIDE;
  virtual void ShelfItemChanged(int model_index,
                                const LauncherItem& old_item) OVERRIDE;
  virtual void ShelfItemMoved(int start_index, int target_index) OVERRIDE;
  virtual void ShelfStatusChanged() OVERRIDE;

  
  virtual void PointerPressedOnButton(views::View* view,
                                      Pointer pointer,
                                      const ui::LocatedEvent& event) OVERRIDE;
  virtual void PointerDraggedOnButton(views::View* view,
                                      Pointer pointer,
                                      const ui::LocatedEvent& event) OVERRIDE;
  virtual void PointerReleasedOnButton(views::View* view,
                                       Pointer pointer,
                                       bool canceled) OVERRIDE;
  virtual void MouseMovedOverButton(views::View* view) OVERRIDE;
  virtual void MouseEnteredButton(views::View* view) OVERRIDE;
  virtual void MouseExitedButton(views::View* view) OVERRIDE;
  virtual base::string16 GetAccessibleName(const views::View* view) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  
  
  
  
  bool ShowListMenuForView(const LauncherItem& item,
                           views::View* source,
                           const ui::Event& event);

  
  virtual void ShowContextMenuForView(views::View* source,
                                      const gfx::Point& point,
                                      ui::MenuSourceType source_type) OVERRIDE;

  
  
  
  
  void ShowMenu(scoped_ptr<views::MenuModelAdapter> menu_model_adapter,
                views::View* source,
                const gfx::Point& click_point,
                bool context_menu,
                ui::MenuSourceType source_type);

  
  virtual void OnBoundsAnimatorProgressed(
      views::BoundsAnimator* animator) OVERRIDE;
  virtual void OnBoundsAnimatorDone(views::BoundsAnimator* animator) OVERRIDE;

  
  
  bool IsUsableEvent(const ui::Event& event);

  
  const LauncherItem* LauncherItemForView(const views::View* view) const;

  
  bool ShouldShowTooltipForView(const views::View* view) const;

  
  
  int CalculateShelfDistance(const gfx::Point& coordinate) const;

  
  ShelfModel* model_;

  
  ShelfDelegate* delegate_;

  
  
  scoped_ptr<views::ViewModel> view_model_;

  
  int first_visible_index_;

  
  
  int last_visible_index_;

  scoped_ptr<views::BoundsAnimator> bounds_animator_;

  OverflowButton* overflow_button_;

  scoped_ptr<OverflowBubble> overflow_bubble_;

  OverflowBubble* owner_overflow_bubble_;

  scoped_ptr<ShelfTooltipManager> tooltip_;

  
  
  Pointer drag_pointer_;

  
  
  views::View* drag_view_;

  
  int drag_offset_;

  
  int start_drag_index_;

  
  LauncherID context_menu_id_;

  scoped_ptr<views::FocusSearch> focus_search_;

  scoped_ptr<views::MenuRunner> launcher_menu_runner_;

  ObserverList<ShelfIconObserver> observers_;

  
  
  int leading_inset_;

  ShelfGestureHandler gesture_handler_;

  
  bool cancelling_drag_model_changed_;

  
  
  int last_hidden_index_;

  
  base::TimeDelta closing_event_time_;

  
  
  bool* got_deleted_;

  
  
  bool drag_and_drop_item_pinned_;

  
  
  LauncherID drag_and_drop_launcher_id_;

  
  std::string drag_and_drop_app_id_;

  
  gfx::Size pre_drag_and_drop_size_;

  
  
  scoped_ptr<ash::internal::DragImageView> drag_image_;

  
  gfx::Vector2d drag_image_offset_;

  
  views::View* drag_replaced_view_;

  
  bool dragged_off_shelf_;

  
  views::View* snap_back_from_rip_off_view_;

  
  ShelfItemDelegateManager* item_manager_;

  
  ShelfLayoutManager* layout_manager_;

  
  bool overflow_mode_;

  
  ShelfView* main_shelf_;

  
  bool dragged_off_from_overflow_to_shelf_;

  DISALLOW_COPY_AND_ASSIGN(ShelfView);
};

}  
}  

#endif  
