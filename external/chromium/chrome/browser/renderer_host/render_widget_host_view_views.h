// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_VIEWS_H_
#define CHROME_BROWSER_RENDERER_HOST_RENDER_WIDGET_HOST_VIEW_VIEWS_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/time.h"
#include "content/browser/renderer_host/render_widget_host_view.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebInputEvent.h"
#include "ui/gfx/native_widget_types.h"
#include "views/controls/native/native_view_host.h"
#include "views/events/event.h"
#include "views/ime/text_input_client.h"
#include "views/view.h"
#include "webkit/glue/webcursor.h"

class RenderWidgetHost;
struct NativeWebKeyboardEvent;

class RenderWidgetHostViewViews : public RenderWidgetHostView,
                                  public views::View,
                                  public views::TextInputClient {
 public:
  
  static const char kViewClassName[];

  explicit RenderWidgetHostViewViews(RenderWidgetHost* widget);
  virtual ~RenderWidgetHostViewViews();

  
  void InitAsChild();

  
  virtual void InitAsPopup(RenderWidgetHostView* parent_host_view,
                           const gfx::Rect& pos) OVERRIDE;
  virtual void InitAsFullscreen() OVERRIDE;
  virtual RenderWidgetHost* GetRenderWidgetHost() const OVERRIDE;
  virtual void DidBecomeSelected() OVERRIDE;
  virtual void WasHidden() OVERRIDE;
  virtual void SetSize(const gfx::Size& size) OVERRIDE;
  virtual void SetBounds(const gfx::Rect& rect) OVERRIDE;
  virtual gfx::NativeView GetNativeView() OVERRIDE;
  virtual void MovePluginWindows(
      const std::vector<webkit::npapi::WebPluginGeometry>& moves) OVERRIDE;
  virtual bool HasFocus() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual bool IsShowing() OVERRIDE;
  virtual gfx::Rect GetViewBounds() const OVERRIDE;
  virtual void UpdateCursor(const WebCursor& cursor) OVERRIDE;
  virtual void SetIsLoading(bool is_loading) OVERRIDE;
  virtual void ImeUpdateTextInputState(WebKit::WebTextInputType type,
                                       const gfx::Rect& caret_rect) OVERRIDE;
  virtual void ImeCancelComposition() OVERRIDE;
  virtual void DidUpdateBackingStore(
      const gfx::Rect& scroll_rect, int scroll_dx, int scroll_dy,
      const std::vector<gfx::Rect>& copy_rects) OVERRIDE;
  virtual void RenderViewGone(base::TerminationStatus status,
                              int error_code) OVERRIDE;
  virtual void Destroy() OVERRIDE;
  virtual void WillDestroyRenderWidget(RenderWidgetHost* rwh) OVERRIDE {}
  virtual void SetTooltipText(const std::wstring& tooltip_text) OVERRIDE;
  virtual void SelectionChanged(const std::string& text) OVERRIDE;
  virtual void ShowingContextMenu(bool showing) OVERRIDE;
  virtual BackingStore* AllocBackingStore(const gfx::Size& size) OVERRIDE;
  virtual void SetBackground(const SkBitmap& background) OVERRIDE;
  virtual void CreatePluginContainer(gfx::PluginWindowHandle id) OVERRIDE;
  virtual void DestroyPluginContainer(gfx::PluginWindowHandle id) OVERRIDE;
  virtual void SetVisuallyDeemphasized(const SkColor* color,
                                       bool animate) OVERRIDE;
  virtual bool ContainsNativeView(gfx::NativeView native_view) const OVERRIDE;
  virtual void AcceleratedCompositingActivated(bool activated) OVERRIDE;
  virtual gfx::PluginWindowHandle GetCompositingSurface() OVERRIDE;

  
  
  
  gfx::NativeView GetInnerNativeView() const;

  
  virtual std::string GetClassName() const OVERRIDE;
  virtual gfx::NativeCursor GetCursorForPoint(ui::EventType type,
                                              const gfx::Point& point) OVERRIDE;
  virtual bool OnMousePressed(const views::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseMoved(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseEntered(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const views::MouseEvent& event) OVERRIDE;
  virtual View::TouchStatus OnTouchEvent(
      const views::TouchEvent& event) OVERRIDE;
  virtual bool OnKeyPressed(const views::KeyEvent& event) OVERRIDE;
  virtual bool OnKeyReleased(const views::KeyEvent& event) OVERRIDE;
  virtual bool OnMouseWheel(const views::MouseWheelEvent& event) OVERRIDE;
  virtual views::TextInputClient* GetTextInputClient() OVERRIDE;

  
  virtual void SetCompositionText(
      const ui::CompositionText& composition) OVERRIDE;
  virtual void ConfirmCompositionText() OVERRIDE;
  virtual void ClearCompositionText() OVERRIDE;
  virtual void InsertText(const string16& text) OVERRIDE;
  virtual void InsertChar(char16 ch, int flags) OVERRIDE;
  virtual ui::TextInputType GetTextInputType() OVERRIDE;
  virtual gfx::Rect GetCaretBounds() OVERRIDE;
  virtual bool HasCompositionText() OVERRIDE;
  virtual bool GetTextRange(ui::Range* range) OVERRIDE;
  virtual bool GetCompositionTextRange(ui::Range* range) OVERRIDE;
  virtual bool GetSelectionRange(ui::Range* range) OVERRIDE;
  virtual bool SetSelectionRange(const ui::Range& range) OVERRIDE;
  virtual bool DeleteRange(const ui::Range& range) OVERRIDE;
  virtual bool GetTextFromRange(
      const ui::Range& range,
      const base::Callback<void(const string16&)>& callback) OVERRIDE;
  virtual void OnInputMethodChanged() OVERRIDE;
  virtual bool ChangeTextDirectionAndLayoutAlignment(
      base::i18n::TextDirection direction) OVERRIDE;
  virtual views::View* GetOwnerViewOfTextInputClient() OVERRIDE;

 protected:
  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void Focus() OVERRIDE;
  virtual void Blur() OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;

 private:
  friend class RenderWidgetHostViewViewsWidget;

  
  
  bool NeedsInputGrab();

  
  
  bool IsPopup();

  
  void ShowCurrentCursor();

  
  WebKit::WebMouseEvent WebMouseEventFromViewsEvent(
      const views::MouseEvent& event);

  
  
  void FinishImeCompositionSession();

  
  RenderWidgetHost* host_;

  
  
  
  bool about_to_validate_and_paint_;

  
  gfx::Rect invalid_rect_;

  
  bool is_hidden_;

  
  bool is_loading_;

  
  WebCursor current_cursor_;

  
  gfx::NativeCursor native_cursor_;

  
  bool is_showing_context_menu_;

  
  
  
  base::TimeTicks whiteout_start_time_;

  
  base::TimeTicks tab_switch_paint_time_;

  
  bool visually_deemphasized_;

  
  gfx::Size requested_size_;

  
  
  
  WebKit::WebTouchEvent touch_event_;

  
  ui::TextInputType text_input_type_;

  
  gfx::Rect caret_bounds_;

  
  bool has_composition_text_;

  DISALLOW_COPY_AND_ASSIGN(RenderWidgetHostViewViews);
};

#endif  