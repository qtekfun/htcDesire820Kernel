// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_VIEW_H_
#define UI_VIEWS_VIEW_H_

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/i18n/rtl.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "build/build_config.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/accessibility/accessibility_types.h"
#include "ui/base/dragdrop/drag_drop_types.h"
#include "ui/base/dragdrop/drop_target_event.h"
#include "ui/base/dragdrop/os_exchange_data.h"
#include "ui/base/ui_base_types.h"
#include "ui/compositor/layer_delegate.h"
#include "ui/compositor/layer_owner.h"
#include "ui/events/event.h"
#include "ui/events/event_target.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/vector2d.h"
#include "ui/views/views_export.h"

#if defined(OS_WIN)
#include "base/win/scoped_comptr.h"
#endif

using ui::OSExchangeData;

namespace gfx {
class Canvas;
class Insets;
class Path;
class Transform;
}

namespace ui {
struct AccessibleViewState;
class Compositor;
class Layer;
class NativeTheme;
class TextInputClient;
class Texture;
class ThemeProvider;
}

namespace views {

class Background;
class Border;
class ContextMenuController;
class DragController;
class FocusManager;
class FocusTraversable;
class InputMethod;
class LayoutManager;
class NativeViewAccessibility;
class ScrollView;
class Widget;

namespace internal {
class PostEventDispatchHandler;
class RootView;
}

class VIEWS_EXPORT View : public ui::LayerDelegate,
                          public ui::LayerOwner,
                          public ui::AcceleratorTarget,
                          public ui::EventTarget {
 public:
  typedef std::vector<View*> Views;

  
  
  
  
  
  
  
  enum HitTestSource {
    HIT_TEST_SOURCE_MOUSE,
    HIT_TEST_SOURCE_TOUCH
  };

  struct ViewHierarchyChangedDetails {
    ViewHierarchyChangedDetails()
        : is_add(false),
          parent(NULL),
          child(NULL),
          move_view(NULL) {}

    ViewHierarchyChangedDetails(bool is_add,
                                View* parent,
                                View* child,
                                View* move_view)
        : is_add(is_add),
          parent(parent),
          child(child),
          move_view(move_view) {}

    bool is_add;
    
    View* parent;
    
    View* child;
    
    
    
    
    
    
    
    
    View* move_view;
  };

  

  View();
  virtual ~View();

  
  void set_owned_by_client() { owned_by_client_ = true; }

  

  
  virtual const Widget* GetWidget() const;
  virtual Widget* GetWidget();

  
  void AddChildView(View* view);
  void AddChildViewAt(View* view, int index);

  
  
  void ReorderChildView(View* view, int index);

  
  void RemoveChildView(View* view);

  
  
  void RemoveAllChildViews(bool delete_children);

  int child_count() const { return static_cast<int>(children_.size()); }
  bool has_children() const { return !children_.empty(); }

  
  const View* child_at(int index) const {
    DCHECK_GE(index, 0);
    DCHECK_LT(index, child_count());
    return children_[index];
  }
  View* child_at(int index) {
    return const_cast<View*>(const_cast<const View*>(this)->child_at(index));
  }

  
  const View* parent() const { return parent_; }
  View* parent() { return parent_; }

  
  
  bool Contains(const View* view) const;

  
  int GetIndexOf(const View* view) const;

  
  
  
  
  
  
  
  
  

  void SetBounds(int x, int y, int width, int height);
  void SetBoundsRect(const gfx::Rect& bounds);
  void SetSize(const gfx::Size& size);
  void SetPosition(const gfx::Point& position);
  void SetX(int x);
  void SetY(int y);

  
  const gfx::Rect& bounds() const { return bounds_; }
  int x() const { return bounds_.x(); }
  int y() const { return bounds_.y(); }
  int width() const { return bounds_.width(); }
  int height() const { return bounds_.height(); }
  const gfx::Size& size() const { return bounds_.size(); }

  
  
  gfx::Rect GetContentsBounds() const;

  
  
  gfx::Rect GetLocalBounds() const;

  
  gfx::Rect GetLayerBoundsInPixel() const;

  
  
  virtual gfx::Insets GetInsets() const;

  
  
  
  
  
  
  
  gfx::Rect GetVisibleBounds() const;

  
  gfx::Rect GetBoundsInScreen() const;

  
  
  virtual int GetBaseline() const;

  
  virtual gfx::Size GetPreferredSize();

  
  void SizeToPreferredSize();

  
  
  virtual gfx::Size GetMinimumSize();

  
  
  virtual gfx::Size GetMaximumSize();

  
  
  
  
  virtual int GetHeightForWidth(int w);

  
  virtual void SetVisible(bool visible);

  
  bool visible() const { return visible_; }

  
  virtual bool IsDrawn() const;

  
  
  
  void SetEnabled(bool enabled);

  
  bool enabled() const { return enabled_; }

  
  
  
  
  
  void SetFillsBoundsOpaquely(bool fills_bounds_opaquely);

  

  

  gfx::Transform GetTransform() const;

  
  void set_clip_insets(gfx::Insets clip_insets) { clip_insets_ = clip_insets; }

  
  void SetTransform(const gfx::Transform& transform);

  
  
  
  
  
  
  void SetPaintToLayer(bool paint_to_layer);

  
  
  
  
  
  ui::Layer* RecreateLayer() WARN_UNUSED_RESULT;

  

  
  
  
  
  
  
  
  gfx::Rect GetMirroredBounds() const;
  gfx::Point GetMirroredPosition() const;
  int GetMirroredX() const;

  
  
  
  
  
  
  
  int GetMirroredXForRect(const gfx::Rect& rect) const;

  
  
  
  
  
  
  
  
  
  
  
  int GetMirroredXInView(int x) const;

  
  
  
  
  
  
  
  
  
  int GetMirroredXWithWidthInView(int x, int w) const;

  

  
  
  virtual void Layout();

  
  
  
  
  void InvalidateLayout();

  
  
  
  
  LayoutManager* GetLayoutManager() const;
  void SetLayoutManager(LayoutManager* layout);

  

  
  static const char kViewClassName[];

  
  
  
  
  virtual const char* GetClassName() const;

  
  
  View* GetAncestorWithClassName(const std::string& name);

  
  
  
  virtual const View* GetViewByID(int id) const;
  virtual View* GetViewByID(int id);

  
  
  int id() const { return id_; }
  void set_id(int id) { id_ = id; }

  
  
  
  
  void SetGroup(int gid);
  
  int GetGroup() const;

  
  
  
  
  virtual bool IsGroupFocusTraversable() const;

  
  
  void GetViewsInGroup(int group, Views* views);

  
  
  
  virtual View* GetSelectedViewForGroup(int group);

  

  
  
  

  
  
  
  
  
  static void ConvertPointToTarget(const View* source,
                                   const View* target,
                                   gfx::Point* point);

  
  
  
  
  
  
  static void ConvertRectToTarget(const View* source,
                                  const View* target,
                                  gfx::RectF* rect);

  
  static void ConvertPointToWidget(const View* src, gfx::Point* point);

  
  
  static void ConvertPointFromWidget(const View* dest, gfx::Point* p);

  
  static void ConvertPointToScreen(const View* src, gfx::Point* point);

  
  static void ConvertPointFromScreen(const View* dst, gfx::Point* point);

  
  
  gfx::Rect ConvertRectToParent(const gfx::Rect& rect) const;

  
  
  gfx::Rect ConvertRectToWidget(const gfx::Rect& rect) const;

  

  
  
  
  
  
  
  virtual void SchedulePaint();
  virtual void SchedulePaintInRect(const gfx::Rect& r);

  
  
  
  
  virtual void Paint(gfx::Canvas* canvas);

  
  void set_background(Background* b);
  const Background* background() const { return background_.get(); }
  Background* background() { return background_.get(); }

  
  void set_border(Border* b);
  const Border* border() const { return border_.get(); }
  Border* border() { return border_.get(); }

  
  virtual ui::ThemeProvider* GetThemeProvider() const;

  
  
  
  ui::NativeTheme* GetNativeTheme() {
    return const_cast<ui::NativeTheme*>(
        const_cast<const View*>(this)->GetNativeTheme());
  }
  const ui::NativeTheme* GetNativeTheme() const;

  

  
  
  
  
  
  
  
  
  bool FlipCanvasOnPaintForRTLUI() const {
    return flip_canvas_on_paint_for_rtl_ui_ ? base::i18n::IsRTL() : false;
  }

  
  
  
  
  
  
  
  
  
  
  void EnableCanvasFlippingForRTLUI(bool enable) {
    flip_canvas_on_paint_for_rtl_ui_ = enable;
  }

  

  
  static void set_use_acceleration_when_possible(bool use);
  static bool get_use_acceleration_when_possible();

  
  
  

  
  
  View* GetEventHandlerForPoint(const gfx::Point& point);

  
  
  
  
  
  
  
  virtual View* GetEventHandlerForRect(const gfx::Rect& rect);

  
  
  
  virtual View* GetTooltipHandlerForPoint(const gfx::Point& point);

  
  
  
  
  
  virtual gfx::NativeCursor GetCursor(const ui::MouseEvent& event);

  
  
  bool HitTestPoint(const gfx::Point& point) const;

  
  virtual bool HitTestRect(const gfx::Rect& rect) const;

  
  
  bool IsMouseHovered();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool OnMousePressed(const ui::MouseEvent& event);

  
  
  
  
  
  
  
  
  
  
  virtual bool OnMouseDragged(const ui::MouseEvent& event);

  
  
  
  
  
  
  virtual void OnMouseReleased(const ui::MouseEvent& event);

  
  
  virtual void OnMouseCaptureLost();

  
  
  
  
  virtual void OnMouseMoved(const ui::MouseEvent& event);

  
  
  
  virtual void OnMouseEntered(const ui::MouseEvent& event);

  
  
  
  virtual void OnMouseExited(const ui::MouseEvent& event);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void SetMouseHandler(View* new_mouse_handler);

  
  
  
  
  virtual bool OnKeyPressed(const ui::KeyEvent& event);
  virtual bool OnKeyReleased(const ui::KeyEvent& event);

  
  
  
  
  virtual bool OnMouseWheel(const ui::MouseWheelEvent& event);


  
  void set_notify_enter_exit_on_child(bool notify) {
    notify_enter_exit_on_child_ = notify;
  }
  bool notify_enter_exit_on_child() const {
    return notify_enter_exit_on_child_;
  }

  
  
  virtual ui::TextInputClient* GetTextInputClient();

  
  
  
  virtual InputMethod* GetInputMethod();
  virtual const InputMethod* GetInputMethod() const;

  
  virtual bool CanAcceptEvent(const ui::Event& event) OVERRIDE;
  virtual ui::EventTarget* GetParentTarget() OVERRIDE;
  virtual scoped_ptr<ui::EventTargetIterator> GetChildIterator() const OVERRIDE;
  virtual ui::EventTargeter* GetEventTargeter() OVERRIDE;

  
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnScrollEvent(ui::ScrollEvent* event) OVERRIDE;
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  

  
  
  
  
  
  virtual void AddAccelerator(const ui::Accelerator& accelerator);

  
  virtual void RemoveAccelerator(const ui::Accelerator& accelerator);

  
  virtual void ResetAccelerators();

  
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;

  
  
  
  virtual bool CanHandleAccelerators() const OVERRIDE;

  

  
  virtual bool HasFocus() const;

  
  View* GetNextFocusableView();
  const View* GetNextFocusableView() const;

  
  View* GetPreviousFocusableView();

  
  
  
  
  
  void SetNextFocusableView(View* view);

  
  
  
  void SetFocusable(bool focusable);

  
  bool focusable() const { return focusable_ && enabled_ && visible_; }

  
  virtual bool IsFocusable() const;

  
  
  bool IsAccessibilityFocusable() const;

  
  
  
  void SetAccessibilityFocusable(bool accessibility_focusable);

  
  
  
  virtual FocusManager* GetFocusManager();
  virtual const FocusManager* GetFocusManager() const;

  
  virtual void RequestFocus();

  
  
  
  virtual void AboutToRequestFocusFromTabTraversal(bool reverse) {}

  
  
  
  
  
  
  
  
  
  virtual bool SkipDefaultKeyEventProcessing(const ui::KeyEvent& event);

  
  
  
  virtual FocusTraversable* GetFocusTraversable();

  
  
  
  
  
  virtual FocusTraversable* GetPaneFocusTraversable();

  

  
  
  
  
  
  
  virtual bool GetTooltipText(const gfx::Point& p, string16* tooltip) const;

  
  
  
  virtual bool GetTooltipTextOrigin(const gfx::Point& p, gfx::Point* loc) const;

  

  
  
  ContextMenuController* context_menu_controller() {
    return context_menu_controller_;
  }
  void set_context_menu_controller(ContextMenuController* menu_controller) {
    context_menu_controller_ = menu_controller;
  }

  
  
  
  
  
  
  virtual void ShowContextMenu(const gfx::Point& p,
                               ui::MenuSourceType source_type);

  
  
  static bool ShouldShowContextMenuOnMousePress();

  

  DragController* drag_controller() { return drag_controller_; }
  void set_drag_controller(DragController* drag_controller) {
    drag_controller_ = drag_controller;
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  
  
  virtual bool GetDropFormats(
      int* formats,
      std::set<OSExchangeData::CustomFormat>* custom_formats);

  
  
  virtual bool AreDropTypesRequired();

  
  
  virtual bool CanDrop(const OSExchangeData& data);

  
  
  
  
  virtual void OnDragEntered(const ui::DropTargetEvent& event);

  
  
  
  
  virtual int OnDragUpdated(const ui::DropTargetEvent& event);

  
  
  virtual void OnDragExited();

  
  
  virtual int OnPerformDrop(const ui::DropTargetEvent& event);

  
  
  virtual void OnDragDone();

  
  
  static bool ExceededDragThreshold(const gfx::Vector2d& delta);

  

  
  virtual void GetAccessibleState(ui::AccessibleViewState* state) { }

  
  virtual gfx::NativeViewAccessible GetNativeViewAccessible();

  
  
  
  
  
  
  void NotifyAccessibilityEvent(ui::AccessibilityTypes::Event event_type,
                                bool send_native_event);

  
  
  

  
  
  
  
  virtual void ScrollRectToVisible(const gfx::Rect& rect);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int GetPageScrollIncrement(ScrollView* scroll_view,
                                     bool is_horizontal, bool is_positive);
  virtual int GetLineScrollIncrement(ScrollView* scroll_view,
                                     bool is_horizontal, bool is_positive);

 protected:
  
  
  struct DragInfo {
    
    
    void Reset();

    
    
    
    void PossibleDrag(const gfx::Point& p);

    
    bool possible_drag;

    
    gfx::Point start_pt;
  };

  

  
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds);

  
  
  virtual void ChildPreferredSizeChanged(View* child) {}

  
  
  virtual void ChildVisibilityChanged(View* child) {}

  
  
  
  virtual void PreferredSizeChanged();

  
  
  
  virtual bool NeedsNotificationWhenVisibleBoundsChange() const;

  
  
  
  virtual void OnVisibleBoundsChanged();

  
  
  virtual void OnEnabledChanged();

  

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void ViewHierarchyChanged(const ViewHierarchyChangedDetails& details);

  
  
  virtual void VisibilityChanged(View* starting_from, bool is_visible);

  
  
  
  
  
  
  virtual void NativeViewHierarchyChanged();

  

  
  
  virtual void PaintChildren(gfx::Canvas* canvas);

  
  
  
  virtual void OnPaint(gfx::Canvas* canvas);

  
  
  
  virtual void OnPaintBackground(gfx::Canvas* canvas);

  
  virtual void OnPaintBorder(gfx::Canvas* canvas);

  

  
  
  virtual gfx::Vector2d CalculateOffsetToAncestorWithLayer(
      ui::Layer** layer_parent);

  
  
  
  virtual void UpdateParentLayer();

  
  
  
  
  
  void MoveLayerToParent(ui::Layer* parent_layer, const gfx::Point& point);

  
  
  void UpdateChildLayerBounds(const gfx::Vector2d& offset);

  
  virtual void OnPaintLayer(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnDeviceScaleFactorChanged(float device_scale_factor) OVERRIDE;
  virtual base::Closure PrepareForLayerBoundsChange() OVERRIDE;

  
  
  
  virtual void ReorderLayers();

  
  
  
  
  
  
  virtual void ReorderChildLayers(ui::Layer* parent_layer);

  

  
  
  
  
  virtual bool HasHitTestMask() const;

  
  
  virtual void GetHitTestMask(HitTestSource source, gfx::Path* mask) const;

  virtual DragInfo* GetDragInfo();

  

  
  virtual void OnFocus();
  virtual void OnBlur();

  
  void Focus();
  void Blur();

  

  
  
  
  
  virtual void OnThemeChanged() {}

  
  
  
  virtual void OnLocaleChanged() {}

  

  
  void TooltipTextChanged();

  

  
  
  
  
  
  
  virtual gfx::Point GetKeyboardContextMenuLocation();

  

  
  
  
  
  virtual int GetDragOperations(const gfx::Point& press_pt);
  virtual void WriteDragData(const gfx::Point& press_pt, OSExchangeData* data);

  
  
  bool InDrag();

  
  
  
  static int GetHorizontalDragThreshold();
  static int GetVerticalDragThreshold();

  

  
  virtual void OnNativeThemeChanged(const ui::NativeTheme* theme) {}

  

#if !defined(NDEBUG)
  
  
  
  
  virtual std::string PrintViewGraph(bool first);

  
  
  
  
  std::string DoPrintViewGraph(bool first, View* view_with_children);
#endif

 private:
  friend class internal::PostEventDispatchHandler;
  friend class internal::RootView;
  friend class FocusManager;
  friend class Widget;

  

  enum SchedulePaintType {
    
    SCHEDULE_PAINT_SIZE_SAME,

    
    SCHEDULE_PAINT_SIZE_CHANGED
  };

  
  
  void SchedulePaintBoundsChanged(SchedulePaintType type);

  
  
  void PaintCommon(gfx::Canvas* canvas);

  

  
  
  
  
  
  
  
  void DoRemoveChildView(View* view,
                         bool update_focus_cycle,
                         bool update_tool_tip,
                         bool delete_removed_view,
                         View* new_parent);

  
  
  
  
  void PropagateRemoveNotifications(View* old_parent, View* new_parent);

  
  void PropagateAddNotifications(const ViewHierarchyChangedDetails& details);

  
  
  void PropagateNativeViewHierarchyChanged();

  
  
  void ViewHierarchyChangedImpl(bool register_accelerators,
                                const ViewHierarchyChangedDetails& details);

  
  void PropagateNativeThemeChanged(const ui::NativeTheme* theme);

  

  
  void PropagateVisibilityNotifications(View* from, bool is_visible);

  
  
  void VisibilityChangedImpl(View* starting_from, bool is_visible);

  
  
  void BoundsChanged(const gfx::Rect& previous_bounds);

  
  
  
  
  
  static void RegisterChildrenForVisibleBoundsNotification(View* view);
  static void UnregisterChildrenForVisibleBoundsNotification(View* view);
  void RegisterForVisibleBoundsNotification();
  void UnregisterForVisibleBoundsNotification();

  
  
  void AddDescendantToNotify(View* view);
  void RemoveDescendantToNotify(View* view);

  
  void SetLayerBounds(const gfx::Rect& bounds_in_dip);

  

  
  
  
  bool GetTransformRelativeTo(const View* ancestor,
                              gfx::Transform* transform) const;

  

  
  
  
  bool ConvertPointForAncestor(const View* ancestor, gfx::Point* point) const;

  
  
  
  
  bool ConvertPointFromAncestor(const View* ancestor, gfx::Point* point) const;

  
  
  
  bool ConvertRectForAncestor(const View* ancestor, gfx::RectF* rect) const;

  
  
  
  
  bool ConvertRectFromAncestor(const View* ancestor, gfx::RectF* rect) const;

  

  
  void CreateLayer();

  
  
  void UpdateParentLayers();

  
  
  
  void ReparentLayer(const gfx::Vector2d& offset, ui::Layer* parent_layer);

  
  
  
  void UpdateLayerVisibility();
  void UpdateChildLayerVisibility(bool visible);

  
  
  void OrphanLayers();

  
  
  void DestroyLayer();

  

  bool ProcessMousePressed(const ui::MouseEvent& event);
  bool ProcessMouseDragged(const ui::MouseEvent& event);
  void ProcessMouseReleased(const ui::MouseEvent& event);

  

  
  
  void RegisterPendingAccelerators();

  
  
  
  void UnregisterAccelerators(bool leave_data_intact);

  

  
  
  void InitFocusSiblings(View* view, int index);

  

  
  
  virtual void PropagateThemeChanged();

  
  
  virtual void PropagateLocaleChanged();

  

  
  
  
  
  
  void UpdateTooltip();

  

  
  
  
  
  
  bool DoDrag(const ui::LocatedEvent& event,
              const gfx::Point& press_pt,
              ui::DragDropTypes::DragEventSource source);

  

  

  
  
  bool owned_by_client_;

  

  
  int id_;

  
  
  
  int group_;

  

  
  View* parent_;

  
  Views children_;

  

  
  gfx::Rect bounds_;

  
  bool visible_;

  
  bool enabled_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  bool notify_enter_exit_on_child_;

  
  
  bool registered_for_visible_bounds_notification_;

  
  scoped_ptr<Views> descendants_to_notify_;

  

  
  
  gfx::Insets clip_insets_;

  

  
  bool needs_layout_;

  
  
  scoped_ptr<LayoutManager> layout_manager_;

  

  
  scoped_ptr<Background> background_;

  
  scoped_ptr<Border> border_;

  

  
  
  
  bool flip_canvas_on_paint_for_rtl_ui_;

  

  bool paint_to_layer_;

  

  
  FocusManager* accelerator_focus_manager_;

  
  
  
  scoped_ptr<std::vector<ui::Accelerator> > accelerators_;
  size_t registered_accelerator_count_;

  

  
  View* next_focusable_view_;

  
  View* previous_focusable_view_;

  
  bool focusable_;

  
  
  bool accessibility_focusable_;

  

  
  ContextMenuController* context_menu_controller_;

  

  DragController* drag_controller_;

  

  scoped_ptr<internal::PostEventDispatchHandler> post_dispatch_handler_;

  

  
  
  NativeViewAccessibility* native_view_accessibility_;

  DISALLOW_COPY_AND_ASSIGN(View);
};

}  

#endif  
