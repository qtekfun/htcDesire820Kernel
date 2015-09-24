// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_WINDOW_H_
#define UI_AURA_WINDOW_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/strings/string16.h"
#include "ui/aura/aura_export.h"
#include "ui/aura/client/window_types.h"
#include "ui/aura/window_layer_type.h"
#include "ui/aura/window_observer.h"
#include "ui/compositor/layer_animator.h"
#include "ui/compositor/layer_delegate.h"
#include "ui/compositor/layer_owner.h"
#include "ui/compositor/layer_type.h"
#include "ui/events/event_constants.h"
#include "ui/events/event_target.h"
#include "ui/events/event_targeter.h"
#include "ui/events/gestures/gesture_types.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"

namespace gfx {
class Display;
class Transform;
class Vector2d;
}

namespace ui {
class EventHandler;
class Layer;
class Texture;
}

namespace aura {

class LayoutManager;
class RootWindow;
class WindowDelegate;
class WindowObserver;

typedef RootWindow WindowEventDispatcher;

template<typename T>
struct WindowProperty;

namespace test {
class WindowTestApi;
}

class AURA_EXPORT Window : public ui::LayerDelegate,
                           public ui::LayerOwner,
                           public ui::EventTarget,
                           public ui::GestureConsumer {
 public:
  
  enum StackDirection {
    STACK_ABOVE,
    STACK_BELOW
  };

  typedef std::vector<Window*> Windows;

  explicit Window(WindowDelegate* delegate);
  virtual ~Window();

  
  void Init(ui::LayerType layer_type);

  
  void InitWithWindowLayerType(WindowLayerType layer_type);

  
  
  
  
  
  ui::Layer* RecreateLayer() WARN_UNUSED_RESULT;

  void set_owned_by_parent(bool owned_by_parent) {
    owned_by_parent_ = owned_by_parent;
  }
  bool owned_by_parent() const { return owned_by_parent_; }

  
  
  
  client::WindowType type() const { return type_; }
  void SetType(client::WindowType type);

  int id() const { return id_; }
  void set_id(int id) { id_ = id; }

  const std::string& name() const { return name_; }
  void SetName(const std::string& name);

  const base::string16 title() const { return title_; }
  void set_title(const base::string16& title) { title_ = title; }

  bool transparent() const { return transparent_; }
  void SetTransparent(bool transparent);

  WindowDelegate* delegate() { return delegate_; }
  const WindowDelegate* delegate() const { return delegate_; }

  const gfx::Rect& bounds() const { return bounds_; }

  Window* parent() { return parent_; }
  const Window* parent() const { return parent_; }

  
  
  
  
  virtual Window* GetRootWindow();
  virtual const Window* GetRootWindow() const;

  WindowEventDispatcher* GetDispatcher();
  const WindowEventDispatcher* GetDispatcher() const;
  void set_dispatcher(WindowEventDispatcher* dispatcher) {
    dispatcher_ = dispatcher;
  }
  bool HasDispatcher() const { return !!dispatcher_; }

  
  void set_user_data(void* user_data) { user_data_ = user_data; }
  void* user_data() const { return user_data_; }

  
  void Show();
  void Hide();
  
  bool IsVisible() const;
  
  
  
  bool TargetVisibility() const { return visible_; }

  
  gfx::Rect GetBoundsInRootWindow() const;

  
  
  
  
  gfx::Rect GetBoundsInScreen() const;

  virtual void SetTransform(const gfx::Transform& transform);

  
  
  void SetLayoutManager(LayoutManager* layout_manager);
  LayoutManager* layout_manager() { return layout_manager_.get(); }

  void set_event_targeter(scoped_ptr<ui::EventTargeter> targeter) {
    targeter_ = targeter.Pass();
  }

  
  
  void SetBounds(const gfx::Rect& new_bounds);

  
  
  void SetBoundsInScreen(const gfx::Rect& new_bounds_in_screen_coords,
                         const gfx::Display& dst_display);

  
  
  gfx::Rect GetTargetBounds() const;

  
  void SchedulePaintInRect(const gfx::Rect& rect);

  
  void StackChildAtTop(Window* child);

  
  
  
  void StackChildAbove(Window* child, Window* target);

  
  void StackChildAtBottom(Window* child);

  
  
  void StackChildBelow(Window* child, Window* target);

  
  void AddChild(Window* child);
  void RemoveChild(Window* child);

  const Windows& children() const { return children_; }

  
  bool Contains(const Window* other) const;

  
  
  
  
  
  
  
  
  void AddTransientChild(Window* child);
  void RemoveTransientChild(Window* child);

  const Windows& transient_children() const { return transient_children_; }

  Window* transient_parent() { return transient_parent_; }
  const Window* transient_parent() const { return transient_parent_; }

  
  
  Window* GetChildById(int id);
  const Window* GetChildById(int id) const;

  
  
  
  static void ConvertPointToTarget(const Window* source,
                                   const Window* target,
                                   gfx::Point* point);

  
  virtual void MoveCursorTo(const gfx::Point& point_in_window);

  
  gfx::NativeCursor GetCursor(const gfx::Point& point) const;

  
  
  
  
  void SetEventFilter(ui::EventHandler* event_filter);

  
  void AddObserver(WindowObserver* observer);
  void RemoveObserver(WindowObserver* observer);
  bool HasObserver(WindowObserver* observer);

  void set_ignore_events(bool ignore_events) { ignore_events_ = ignore_events; }
  bool ignore_events() const { return ignore_events_; }

  
  
  
  
  void SetHitTestBoundsOverrideOuter(const gfx::Insets& mouse_insets,
                                     const gfx::Insets& touch_insets) {
    hit_test_bounds_override_outer_mouse_ = mouse_insets;
    hit_test_bounds_override_outer_touch_ = touch_insets;
  }

  gfx::Insets hit_test_bounds_override_outer_touch() const {
    return hit_test_bounds_override_outer_touch_;
  }

  gfx::Insets hit_test_bounds_override_outer_mouse() const {
    return hit_test_bounds_override_outer_mouse_;
  }

  
  
  
  void set_hit_test_bounds_override_inner(const gfx::Insets& insets) {
    hit_test_bounds_override_inner_ = insets;
  }
  gfx::Insets hit_test_bounds_override_inner() const {
    return hit_test_bounds_override_inner_;
  }

  
  
  
  bool ContainsPointInRoot(const gfx::Point& point_in_root) const;

  
  
  bool ContainsPoint(const gfx::Point& local_point) const;

  
  
  
  
  
  bool HitTest(const gfx::Point& local_point);

  
  
  Window* GetEventHandlerForPoint(const gfx::Point& local_point);

  
  Window* GetTopWindowContainingPoint(const gfx::Point& local_point);

  
  
  Window* GetToplevelWindow();

  
  void Focus();
  void Blur();

  
  bool HasFocus() const;

  
  virtual bool CanFocus() const;

  
  virtual bool CanReceiveEvents() const;

  
  
  void SetCapture();

  
  void ReleaseCapture();

  
  bool HasCapture();

  
  
  
  void SuppressPaint();

  
  
  
  template<typename T>
  void SetProperty(const WindowProperty<T>* property, T value);

  
  
  template<typename T>
  T GetProperty(const WindowProperty<T>* property) const;

  
  
  template<typename T>
  void ClearProperty(const WindowProperty<T>* property);

  
  
  
  void SetNativeWindowProperty(const char* key, void* value);
  void* GetNativeWindowProperty(const char* key) const;

  
  typedef void (*PropertyDeallocator)(int64 value);

  
  virtual void OnDeviceScaleFactorChanged(float device_scale_factor) OVERRIDE;

#if !defined(NDEBUG)
  
  std::string GetDebugInfo() const;
  void PrintWindowHierarchy(int depth) const;
#endif

 protected:
  
  
  void RemoveOrDestroyChildren();

 private:
  friend class test::WindowTestApi;
  friend class LayoutManager;
  friend class RootWindow;
  friend class WindowTargeter;

  
  int64 SetPropertyInternal(const void* key,
                            const char* name,
                            PropertyDeallocator deallocator,
                            int64 value,
                            int64 default_value);
  int64 GetPropertyInternal(const void* key, int64 default_value) const;

  
  void SetBoundsInternal(const gfx::Rect& new_bounds);

  
  
  void SetVisible(bool visible);

  
  void SchedulePaint();

  
  
  void Paint(gfx::Canvas* canvas);

  
  void PaintLayerlessChildren(gfx::Canvas* canvas);

  
  
  
  
  
  Window* GetWindowForPoint(const gfx::Point& local_point,
                            bool return_tightest,
                            bool for_event_handling);

  
  
  
  void RemoveChildImpl(Window* child, Window* new_parent);

  
  
  
  void UnparentLayers(bool has_layerless_ancestor,
                      const gfx::Vector2d& offset);

  
  
  
  
  void ReparentLayers(ui::Layer* parent, const gfx::Vector2d& offset);

  
  
  void OffsetLayerBounds(const gfx::Vector2d& offset);

  
  void OnParentChanged();

  
  bool HasTransientAncestor(const Window* ancestor) const;

  
  
  void SkipNullDelegatesForStacking(StackDirection direction,
                                    Window** target) const;

  
  
  void StackChildRelativeTo(Window* child,
                            Window* target,
                            StackDirection direction);

  
  void StackChildRelativeToImpl(Window* child,
                                Window* target,
                                StackDirection direction);

  
  
  void StackChildLayerRelativeTo(Window* child,
                                 Window* target,
                                 StackDirection direction);

  
  void OnStackingChanged();

  
  
  void NotifyRemovingFromRootWindow();
  void NotifyAddedToRootWindow();

  
  
  void NotifyWindowHierarchyChange(
      const WindowObserver::HierarchyChangeParams& params);
  
  void NotifyWindowHierarchyChangeDown(
      const WindowObserver::HierarchyChangeParams& params);
  
  void NotifyWindowHierarchyChangeUp(
      const WindowObserver::HierarchyChangeParams& params);
  
  void NotifyWindowHierarchyChangeAtReceiver(
      const WindowObserver::HierarchyChangeParams& params);

  
  
  void NotifyWindowVisibilityChanged(aura::Window* target, bool visible);
  
  
  bool NotifyWindowVisibilityChangedAtReceiver(aura::Window* target,
                                               bool visible);
  
  
  
  bool NotifyWindowVisibilityChangedDown(aura::Window* target, bool visible);
  
  void NotifyWindowVisibilityChangedUp(aura::Window* target, bool visible);

  
  
  
  
  
  void OnWindowBoundsChanged(const gfx::Rect& old_bounds, bool contained_mouse);

  
  virtual void OnPaintLayer(gfx::Canvas* canvas) OVERRIDE;
  virtual base::Closure PrepareForLayerBoundsChange() OVERRIDE;

  
  virtual bool CanAcceptEvent(const ui::Event& event) OVERRIDE;
  virtual EventTarget* GetParentTarget() OVERRIDE;
  virtual scoped_ptr<ui::EventTargetIterator> GetChildIterator() const OVERRIDE;
  virtual ui::EventTargeter* GetEventTargeter() OVERRIDE;
  virtual void ConvertEventToTarget(ui::EventTarget* target,
                                    ui::LocatedEvent* event) OVERRIDE;

  
  void UpdateLayerName(const std::string& name);

  
  bool ContainsMouse();

  
  
  
  Window* GetAncestorWithLayer(gfx::Vector2d* offset) {
    return const_cast<Window*>(
        const_cast<const Window*>(this)->GetAncestorWithLayer(offset));
  }
  const Window* GetAncestorWithLayer(gfx::Vector2d* offset) const;

  
  
  
  
  gfx::Rect bounds_;

  WindowEventDispatcher* dispatcher_;

  client::WindowType type_;

  
  
  bool owned_by_parent_;

  WindowDelegate* delegate_;

  
  Window* parent_;

  
  Windows children_;

  
  Windows transient_children_;

  Window* transient_parent_;

  
  
  
  bool visible_;

  int id_;
  std::string name_;

  base::string16 title_;

  
  bool transparent_;

  scoped_ptr<ui::EventHandler> event_filter_;
  scoped_ptr<LayoutManager> layout_manager_;
  scoped_ptr<ui::EventTargeter> targeter_;

  void* user_data_;

  
  bool ignore_events_;

  
  gfx::Insets hit_test_bounds_override_outer_mouse_;
  gfx::Insets hit_test_bounds_override_outer_touch_;
  gfx::Insets hit_test_bounds_override_inner_;

  ObserverList<WindowObserver, true> observers_;

  
  
  
  struct Value {
    const char* name;
    int64 value;
    PropertyDeallocator deallocator;
  };

  std::map<const void*, Value> prop_map_;

  DISALLOW_COPY_AND_ASSIGN(Window);
};

}  

#endif  
