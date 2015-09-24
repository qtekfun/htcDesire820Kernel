// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_WIDGET_H_
#define UI_VIEWS_WIDGET_WIDGET_H_

#include <set>
#include <stack>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "ui/base/ui_base_types.h"
#include "ui/compositor/layer_type.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "ui/views/focus/focus_manager.h"
#include "ui/views/widget/native_widget_delegate.h"
#include "ui/views/window/client_view.h"
#include "ui/views/window/non_client_view.h"

#if defined(OS_WIN)
#if defined(IsMaximized)
#undef IsMaximized
#endif
#if defined(IsMinimized)
#undef IsMinimized
#endif
#if defined(CreateWindow)
#undef CreateWindow
#endif
#endif

namespace gfx {
class Canvas;
class Point;
class Rect;
}

namespace ui {
class Accelerator;
class Compositor;
class DefaultThemeProvider;
class Layer;
class NativeTheme;
class OSExchangeData;
class ThemeProvider;
}

namespace views {

class DesktopRootWindowHost;
class InputMethod;
class NativeWidget;
class NonClientFrameView;
class TooltipManager;
class View;
class WidgetDelegate;
class WidgetObserver;

namespace internal {
class NativeWidgetPrivate;
class RootView;
}

class VIEWS_EXPORT Widget : public internal::NativeWidgetDelegate,
                            public FocusTraversable {
 public:
  typedef std::set<Widget*> Widgets;

  enum FrameType {
    FRAME_TYPE_DEFAULT,         
    FRAME_TYPE_FORCE_CUSTOM,    
    FRAME_TYPE_FORCE_NATIVE     
  };

  
  enum MoveLoopResult {
    
    MOVE_LOOP_SUCCESSFUL,

    
    MOVE_LOOP_CANCELED
  };

  
  enum MoveLoopSource {
    MOVE_LOOP_SOURCE_MOUSE,
    MOVE_LOOP_SOURCE_TOUCH,
  };

  
  enum MoveLoopEscapeBehavior {
    
    MOVE_LOOP_ESCAPE_BEHAVIOR_HIDE,

    
    MOVE_LOOP_ESCAPE_BEHAVIOR_DONT_HIDE,
  };

  struct VIEWS_EXPORT InitParams {
    enum Type {
      TYPE_WINDOW,      
                        
      TYPE_PANEL,       
                        
      TYPE_WINDOW_FRAMELESS,
                        
      TYPE_CONTROL,     
      TYPE_POPUP,       
      TYPE_MENU,        
                        
      TYPE_TOOLTIP,
      TYPE_BUBBLE,
      TYPE_DRAG,        
                        
    };

    enum WindowOpacity {
      
      
      
      INFER_OPACITY,
      
      OPAQUE_WINDOW,
      
      TRANSLUCENT_WINDOW,
    };

    enum Ownership {
      
      
      
      NATIVE_WIDGET_OWNS_WIDGET,
      
      
      WIDGET_OWNS_NATIVE_WIDGET
    };

    InitParams();
    explicit InitParams(Type type);
    ~InitParams();

    
    
    gfx::NativeView GetContext() const;

    Type type;
    
    WidgetDelegate* delegate;
    bool child;
    
    
    
    
    
    WindowOpacity opacity;
    bool accept_events;
    bool can_activate;
    bool keep_on_top;
    Ownership ownership;
    bool mirror_origin_in_rtl;
    bool has_dropshadow;
    
    
    
    bool remove_standard_frame;
    
    
    bool use_system_default_icon;
    
    ui::WindowShowState show_state;
    
    bool double_buffer;
    gfx::NativeView parent;
    
    
    
    
    gfx::Rect bounds;
    
    
    NativeWidget* native_widget;
    
    
    
    
    DesktopRootWindowHost* desktop_root_window_host;
    
    
    
    bool top_level;
    
    
    ui::LayerType layer_type;
    
    
    
    
    
    
    
    gfx::NativeView context;
    
    
    
    std::string wm_role_name;
    std::string wm_class_name;
    std::string wm_class_class;
  };

  Widget();
  virtual ~Widget();

  
  
  
  
  
  
  
  static Widget* CreateWindow(WidgetDelegate* delegate);
  static Widget* CreateWindowWithBounds(WidgetDelegate* delegate,
                                        const gfx::Rect& bounds);

  
  static Widget* CreateWindowWithParent(WidgetDelegate* delegate,
                                        gfx::NativeWindow parent);
  static Widget* CreateWindowWithParentAndBounds(WidgetDelegate* delegate,
                                                 gfx::NativeWindow parent,
                                                 const gfx::Rect& bounds);

  
  static Widget* CreateWindowWithContext(WidgetDelegate* delegate,
                                         gfx::NativeView context);
  static Widget* CreateWindowWithContextAndBounds(WidgetDelegate* delegate,
                                                  gfx::NativeView context,
                                                  const gfx::Rect& bounds);

  
  static Widget* CreateWindowAsFramelessChild(WidgetDelegate* widget_delegate,
                                              gfx::NativeView parent);

  
  
  
  static void NotifyLocaleChanged();

  
  
  static void CloseAllSecondaryWidgets();

  
  
  
  
  static bool ConvertRect(const Widget* source,
                          const Widget* target,
                          gfx::Rect* rect);

  
  
  
  static Widget* GetWidgetForNativeView(gfx::NativeView native_view);
  static Widget* GetWidgetForNativeWindow(gfx::NativeWindow native_window);

  
  
  
  
  
  static Widget* GetTopLevelWidgetForNativeView(gfx::NativeView native_view);

  
  
  static void GetAllChildWidgets(gfx::NativeView native_view,
                                 Widgets* children);

  
  static void GetAllOwnedWidgets(gfx::NativeView native_view,
                                 Widgets* owned);

  
  
  static void ReparentNativeView(gfx::NativeView native_view,
                                 gfx::NativeView new_parent);

  
  
  
  
  
  static int GetLocalizedContentsWidth(int col_resource_id);
  static int GetLocalizedContentsHeight(int row_resource_id);
  static gfx::Size GetLocalizedContentsSize(int col_resource_id,
                                            int row_resource_id);

  
  static bool RequiresNonClientView(InitParams::Type type);

  void Init(const InitParams& params);

  
  gfx::NativeView GetNativeView() const;

  
  
  
  gfx::NativeWindow GetNativeWindow() const;

  
  void AddObserver(WidgetObserver* observer);
  void RemoveObserver(WidgetObserver* observer);
  bool HasObserver(WidgetObserver* observer);

  
  
  virtual bool GetAccelerator(int cmd_id, ui::Accelerator* accelerator);

  
  void ViewHierarchyChanged(const View::ViewHierarchyChangedDetails& details);

  
  
  void NotifyNativeViewHierarchyWillChange();

  
  
  void NotifyNativeViewHierarchyChanged();

  
  
  
  Widget* GetTopLevelWidget();
  const Widget* GetTopLevelWidget() const;

  
  WidgetDelegate* widget_delegate() const { return widget_delegate_; }

  
  
  
  
  void SetContentsView(View* view);
  View* GetContentsView();

  
  gfx::Rect GetWindowBoundsInScreen() const;

  
  gfx::Rect GetClientAreaBoundsInScreen() const;

  
  gfx::Rect GetRestoredBounds() const;

  
  void SetBounds(const gfx::Rect& bounds);
  void SetSize(const gfx::Size& size);

  
  void CenterWindow(const gfx::Size& size);

  
  
  
  void SetBoundsConstrained(const gfx::Rect& bounds);

  
  
  void SetVisibilityChangedAnimationsEnabled(bool value);

  
  
  
  
  
  MoveLoopResult RunMoveLoop(const gfx::Vector2d& drag_offset,
                             MoveLoopSource source,
                             MoveLoopEscapeBehavior escape_behavior);

  
  void EndMoveLoop();

  
  void StackAboveWidget(Widget* widget);
  void StackAbove(gfx::NativeView native_view);
  void StackAtTop();

  
  void StackBelow(gfx::NativeView native_view);

  
  
  void SetShape(gfx::NativeRegion shape);

  
  virtual void Close();

  
  
  
  
  void CloseNow();

  
  
  bool IsClosed() const;

  
  virtual void Show();
  void Hide();

  
  void ShowInactive();

  
  void Activate();

  
  
  void Deactivate();

  
  virtual bool IsActive() const;

  
  
  
  
  void DisableInactiveRendering();

  
  void SetAlwaysOnTop(bool on_top);

  
  
  bool IsAlwaysOnTop() const;

  
  void Maximize();
  void Minimize();
  void Restore();

  
  virtual bool IsMaximized() const;
  bool IsMinimized() const;

  
  void SetFullscreen(bool fullscreen);
  bool IsFullscreen() const;

  
  
  
  void SetOpacity(unsigned char opacity);

  
  
  void SetUseDragFrame(bool use_drag_frame);

  
  
  void FlashFrame(bool flash);

  
  
  View* GetRootView();
  const View* GetRootView() const;

  
  
  
  
  
  void set_is_secondary_widget(bool is_secondary_widget) {
    is_secondary_widget_ = is_secondary_widget;
  }
  bool is_secondary_widget() const { return is_secondary_widget_; }

  
  virtual bool IsVisible() const;

  
  virtual ui::ThemeProvider* GetThemeProvider() const;

  ui::NativeTheme* GetNativeTheme() {
    return const_cast<ui::NativeTheme*>(
        const_cast<const Widget*>(this)->GetNativeTheme());
  }
  const ui::NativeTheme* GetNativeTheme() const;

  
  
  FocusManager* GetFocusManager();
  const FocusManager* GetFocusManager() const;

  
  
  InputMethod* GetInputMethod();
  const InputMethod* GetInputMethod() const;

  
  
  
  
  
  
  void RunShellDrag(View* view,
                    const ui::OSExchangeData& data,
                    const gfx::Point& location,
                    int operation,
                    ui::DragDropTypes::DragEventSource source);

  
  
  View* dragged_view() { return dragged_view_; }

  
  
  virtual void SchedulePaintInRect(const gfx::Rect& rect);

  
  
  void SetCursor(gfx::NativeCursor cursor);

  
  bool IsMouseEventsEnabled() const;

  
  
  
  void SetNativeWindowProperty(const char* name, void* value);
  void* GetNativeWindowProperty(const char* name) const;

  
  void UpdateWindowTitle();

  
  void UpdateWindowIcon();

  
  FocusTraversable* GetFocusTraversable();

  
  
  void ThemeChanged();

  
  
  void LocaleChanged();

  void SetFocusTraversableParent(FocusTraversable* parent);
  void SetFocusTraversableParentView(View* parent_view);

  
  void ClearNativeFocus();

  void set_frame_type(FrameType frame_type) { frame_type_ = frame_type; }
  FrameType frame_type() const { return frame_type_; }

  
  
  
  
  virtual NonClientFrameView* CreateNonClientFrameView();

  
  bool ShouldUseNativeFrame() const;

  
  
  void DebugToggleFrameType();

  
  void FrameTypeChanged();

  NonClientView* non_client_view() {
    return const_cast<NonClientView*>(
        const_cast<const Widget*>(this)->non_client_view());
  }
  const NonClientView* non_client_view() const {
    return non_client_view_;
  }

  ClientView* client_view() {
    return const_cast<ClientView*>(
        const_cast<const Widget*>(this)->client_view());
  }
  const ClientView* client_view() const {
    
    return non_client_view_ ? non_client_view_->client_view() : NULL;
  }

  const ui::Compositor* GetCompositor() const;
  ui::Compositor* GetCompositor();

  
  ui::Layer* GetLayer();

  
  
  
  
  
  
  
  void ReorderNativeViews();

  
  
  void UpdateRootLayers();

  const NativeWidget* native_widget() const;
  NativeWidget* native_widget();

  internal::NativeWidgetPrivate* native_widget_private() {
    return native_widget_;
  }
  const internal::NativeWidgetPrivate* native_widget_private() const {
    return native_widget_;
  }

  
  
  
  
  void SetCapture(View* view);

  
  void ReleaseCapture();

  
  bool HasCapture();

  void set_auto_release_capture(bool auto_release_capture) {
    auto_release_capture_ = auto_release_capture;
  }

  
  TooltipManager* GetTooltipManager();
  const TooltipManager* GetTooltipManager() const;

  
  
  
  
  bool SetInitialFocus();

  void set_focus_on_creation(bool focus_on_creation) {
    focus_on_creation_ = focus_on_creation;
  }

  
  
  
  
  bool is_top_level() const { return is_top_level_; }

  
  bool movement_disabled() const { return movement_disabled_; }
  void set_movement_disabled(bool disabled) { movement_disabled_ = disabled; }

  
  gfx::Rect GetWorkAreaBoundsInScreen() const;

  
  
  void SynthesizeMouseMoveEvent();

  
  
  void OnRootViewLayout();

  
  
  
  
  virtual void OnOwnerClosing();

  
  virtual bool IsModal() const OVERRIDE;
  virtual bool IsDialogBox() const OVERRIDE;
  virtual bool CanActivate() const OVERRIDE;
  virtual bool IsInactiveRenderingDisabled() const OVERRIDE;
  virtual void EnableInactiveRendering() OVERRIDE;
  virtual void OnNativeWidgetActivationChanged(bool active) OVERRIDE;
  virtual void OnNativeFocus(gfx::NativeView old_focused_view) OVERRIDE;
  virtual void OnNativeBlur(gfx::NativeView new_focused_view) OVERRIDE;
  virtual void OnNativeWidgetVisibilityChanging(bool visible) OVERRIDE;
  virtual void OnNativeWidgetVisibilityChanged(bool visible) OVERRIDE;
  virtual void OnNativeWidgetCreated(bool desktop_widget) OVERRIDE;
  virtual void OnNativeWidgetDestroying() OVERRIDE;
  virtual void OnNativeWidgetDestroyed() OVERRIDE;
  virtual gfx::Size GetMinimumSize() OVERRIDE;
  virtual gfx::Size GetMaximumSize() OVERRIDE;
  virtual void OnNativeWidgetMove() OVERRIDE;
  virtual void OnNativeWidgetSizeChanged(const gfx::Size& new_size) OVERRIDE;
  virtual void OnNativeWidgetBeginUserBoundsChange() OVERRIDE;
  virtual void OnNativeWidgetEndUserBoundsChange() OVERRIDE;
  virtual bool HasFocusManager() const OVERRIDE;
  virtual bool OnNativeWidgetPaintAccelerated(
      const gfx::Rect& dirty_region) OVERRIDE;
  virtual void OnNativeWidgetPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual int GetNonClientComponent(const gfx::Point& point) OVERRIDE;
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;
  virtual void OnScrollEvent(ui::ScrollEvent* event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual bool ExecuteCommand(int command_id) OVERRIDE;
  virtual InputMethod* GetInputMethodDirect() OVERRIDE;
  virtual const std::vector<ui::Layer*>& GetRootLayers() OVERRIDE;
  virtual bool HasHitTestMask() const OVERRIDE;
  virtual void GetHitTestMask(gfx::Path* mask) const OVERRIDE;
  virtual Widget* AsWidget() OVERRIDE;
  virtual const Widget* AsWidget() const OVERRIDE;

  
  virtual FocusSearch* GetFocusSearch() OVERRIDE;
  virtual FocusTraversable* GetFocusTraversableParent() OVERRIDE;
  virtual View* GetFocusTraversableParentView() OVERRIDE;

 protected:
  
  
  
  virtual internal::RootView* CreateRootView();

  
  
  
  void DestroyRootView();

 private:
  friend class ComboboxTest;
  friend class NativeTextfieldViewsTest;

  
  
  void SetInactiveRenderingDisabled(bool value);

  
  
  void SaveWindowPlacement();

  
  void SetInitialBounds(const gfx::Rect& bounds);

  
  void SetInitialBoundsForFramelessWindow(const gfx::Rect& bounds);

  
  
  bool GetSavedWindowPlacement(gfx::Rect* bounds,
                               ui::WindowShowState* show_state);

  
  scoped_ptr<InputMethod> CreateInputMethod();

  
  
  
  void ReplaceInputMethod(InputMethod* input_method);

  internal::NativeWidgetPrivate* native_widget_;

  ObserverList<WidgetObserver> observers_;

  
  
  WidgetDelegate* widget_delegate_;

  
  
  
  scoped_ptr<internal::RootView> root_view_;

  
  
  
  
  NonClientView* non_client_view_;

  
  
  
  
  scoped_ptr<FocusManager> focus_manager_;

  
  scoped_ptr<ui::DefaultThemeProvider> default_theme_provider_;

  
  
  View* dragged_view_;

  
  InitParams::Ownership ownership_;

  
  bool is_secondary_widget_;

  
  
  FrameType frame_type_;

  
  
  bool disable_inactive_rendering_;

  
  bool widget_closed_;

  
  
  ui::WindowShowState saved_show_state_;

  
  
  gfx::Rect initial_restored_bounds_;

  
  
  
  bool focus_on_creation_;

  mutable scoped_ptr<InputMethod> input_method_;

  
  bool is_top_level_;

  
  bool native_widget_initialized_;

  
  bool native_widget_destroyed_;

  
  
  bool is_mouse_button_pressed_;

  
  bool is_touch_down_;

  
  
  
  
  bool last_mouse_event_was_move_;
  gfx::Point last_mouse_event_position_;

  
  
  bool auto_release_capture_;

  
  std::vector<ui::Layer*> root_layers_;

  
  bool root_layers_dirty_;

  
  
  bool movement_disabled_;

  DISALLOW_COPY_AND_ASSIGN(Widget);
};

}  

#endif  
