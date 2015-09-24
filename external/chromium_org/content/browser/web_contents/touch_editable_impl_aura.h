// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WEB_CONTENTS_TOUCH_EDITABLE_IMPL_AURA_H_
#define CONTENT_BROWSER_WEB_CONTENTS_TOUCH_EDITABLE_IMPL_AURA_H_

#include <deque>
#include <map>

#include "content/browser/renderer_host/render_widget_host_view_aura.h"
#include "ui/aura/window_observer.h"
#include "ui/base/touch/touch_editing_controller.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"

namespace ui {
class Accelerator;
}

namespace content {
class TouchEditableImplAuraTest;

class CONTENT_EXPORT TouchEditableImplAura
    : public ui::TouchEditable,
      public NON_EXPORTED_BASE(RenderWidgetHostViewAura::TouchEditingClient) {
 public:
  virtual ~TouchEditableImplAura();

  static TouchEditableImplAura* Create();

  void AttachToView(RenderWidgetHostViewAura* view);

  
  
  void UpdateEditingController();

  void OverscrollStarted();
  void OverscrollCompleted();

  
  virtual void StartTouchEditing() OVERRIDE;
  virtual void EndTouchEditing() OVERRIDE;
  virtual void OnSelectionOrCursorChanged(const gfx::Rect& anchor,
                                          const gfx::Rect& focus) OVERRIDE;
  virtual void OnTextInputTypeChanged(ui::TextInputType type) OVERRIDE;
  virtual bool HandleInputEvent(const ui::Event* event) OVERRIDE;
  virtual void GestureEventAck(int gesture_event_type) OVERRIDE;
  virtual void OnViewDestroyed() OVERRIDE;

  
  virtual void SelectRect(const gfx::Point& start,
                          const gfx::Point& end) OVERRIDE;
  virtual void MoveCaretTo(const gfx::Point& point) OVERRIDE;
  virtual void GetSelectionEndPoints(gfx::Rect* p1, gfx::Rect* p2) OVERRIDE;
  virtual gfx::Rect GetBounds() OVERRIDE;
  virtual gfx::NativeView GetNativeView() OVERRIDE;
  virtual void ConvertPointToScreen(gfx::Point* point) OVERRIDE;
  virtual void ConvertPointFromScreen(gfx::Point* point) OVERRIDE;
  virtual bool DrawsHandles() OVERRIDE;
  virtual void OpenContextMenu(const gfx::Point& anchor) OVERRIDE;
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

 protected:
  TouchEditableImplAura();

 private:
  friend class TouchEditableImplAuraTest;

  void Cleanup();

  
  gfx::Rect selection_anchor_rect_;
  gfx::Rect selection_focus_rect_;

  
  ui::TextInputType text_input_type_;

  RenderWidgetHostViewAura* rwhva_;
  scoped_ptr<ui::TouchSelectionController> touch_selection_controller_;

  
  
  bool selection_gesture_in_process_;

  
  
  bool handles_hidden_due_to_scroll_;

  
  bool scroll_in_progress_;

  
  bool overscroll_in_progress_;

  
  bool is_tap_on_focused_textfield_;

  
  
  
  
  std::queue<int> tap_gesture_tap_count_queue_;

  DISALLOW_COPY_AND_ASSIGN(TouchEditableImplAura);
};

}  

#endif  
