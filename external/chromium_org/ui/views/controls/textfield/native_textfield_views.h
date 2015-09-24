// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TEXTFIELD_NATIVE_TEXTFIELD_VIEWS_H_
#define UI_VIEWS_CONTROLS_TEXTFIELD_NATIVE_TEXTFIELD_VIEWS_H_

#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "base/timer/timer.h"
#include "ui/base/ime/text_input_client.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/base/touch/touch_editing_controller.h"
#include "ui/events/event_constants.h"
#include "ui/gfx/font.h"
#include "ui/views/border.h"
#include "ui/views/context_menu_controller.h"
#include "ui/views/controls/textfield/native_textfield_wrapper.h"
#include "ui/views/controls/textfield/textfield_views_model.h"
#include "ui/views/drag_controller.h"
#include "ui/views/view.h"

namespace base {
class Time;
}

namespace gfx {
class Canvas;
}

namespace views {

class FocusableBorder;
class MenuModelAdapter;
class MenuRunner;

class VIEWS_EXPORT NativeTextfieldViews : public View,
                                          public ui::TouchEditable,
                                          public ContextMenuController,
                                          public DragController,
                                          public NativeTextfieldWrapper,
                                          public ui::TextInputClient,
                                          public TextfieldViewsModel::Delegate {
 public:
  explicit NativeTextfieldViews(Textfield* parent);
  virtual ~NativeTextfieldViews();

  
  virtual gfx::NativeCursor GetCursor(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual bool GetDropFormats(
      int* formats,
      std::set<ui::OSExchangeData::CustomFormat>* custom_formats) OVERRIDE;
  virtual bool CanDrop(const ui::OSExchangeData& data) OVERRIDE;
  virtual int OnDragUpdated(const ui::DropTargetEvent& event) OVERRIDE;
  virtual void OnDragExited() OVERRIDE;
  virtual int OnPerformDrop(const ui::DropTargetEvent& event) OVERRIDE;
  virtual void OnDragDone() OVERRIDE;
  virtual bool OnKeyReleased(const ui::KeyEvent& event) OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;
  virtual void OnNativeThemeChanged(const ui::NativeTheme* theme) OVERRIDE;

  
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

  
  virtual void ShowContextMenuForView(View* source,
                                      const gfx::Point& point,
                                      ui::MenuSourceType source_type) OVERRIDE;

  
  virtual void WriteDragDataForView(View* sender,
                                    const gfx::Point& press_pt,
                                    ui::OSExchangeData* data) OVERRIDE;
  virtual int GetDragOperationsForView(View* sender,
                                       const gfx::Point& p) OVERRIDE;
  virtual bool CanStartDragForView(View* sender,
                                   const gfx::Point& press_pt,
                                   const gfx::Point& p) OVERRIDE;

  
  virtual string16 GetText() const OVERRIDE;
  virtual void UpdateText() OVERRIDE;
  virtual void AppendText(const string16& text) OVERRIDE;
  virtual void InsertOrReplaceText(const string16& text) OVERRIDE;
  virtual base::i18n::TextDirection GetTextDirection() const OVERRIDE;
  virtual string16 GetSelectedText() const OVERRIDE;
  virtual void SelectAll(bool reversed) OVERRIDE;
  virtual void ClearSelection() OVERRIDE;
  virtual void UpdateBorder() OVERRIDE;
  virtual void UpdateTextColor() OVERRIDE;
  virtual void UpdateBackgroundColor() OVERRIDE;
  virtual void UpdateReadOnly() OVERRIDE;
  virtual void UpdateFont() OVERRIDE;
  virtual void UpdateIsObscured() OVERRIDE;
  virtual void UpdateEnabled() OVERRIDE;
  virtual gfx::Insets CalculateInsets() OVERRIDE;
  virtual void UpdateHorizontalMargins() OVERRIDE;
  virtual void UpdateVerticalMargins() OVERRIDE;
  virtual bool SetFocus() OVERRIDE;
  virtual View* GetView() OVERRIDE;
  virtual gfx::NativeView GetTestingHandle() const OVERRIDE;
  virtual bool IsIMEComposing() const OVERRIDE;
  virtual gfx::Range GetSelectedRange() const OVERRIDE;
  virtual void SelectRange(const gfx::Range& range) OVERRIDE;
  virtual gfx::SelectionModel GetSelectionModel() const OVERRIDE;
  virtual void SelectSelectionModel(const gfx::SelectionModel& sel) OVERRIDE;
  virtual size_t GetCursorPosition() const OVERRIDE;
  virtual bool GetCursorEnabled() const OVERRIDE;
  virtual void SetCursorEnabled(bool enabled) OVERRIDE;
  virtual bool HandleKeyPressed(const ui::KeyEvent& e) OVERRIDE;
  virtual bool HandleKeyReleased(const ui::KeyEvent& e) OVERRIDE;
  virtual void HandleFocus() OVERRIDE;
  virtual void HandleBlur() OVERRIDE;
  virtual ui::TextInputClient* GetTextInputClient() OVERRIDE;
  virtual void SetColor(SkColor value) OVERRIDE;
  virtual void ApplyColor(SkColor value, const gfx::Range& range) OVERRIDE;
  virtual void SetStyle(gfx::TextStyle style, bool value) OVERRIDE;
  virtual void ApplyStyle(gfx::TextStyle style,
                          bool value,
                          const gfx::Range& range) OVERRIDE;
  virtual void ClearEditHistory() OVERRIDE;
  virtual int GetFontHeight() OVERRIDE;
  virtual int GetTextfieldBaseline() const OVERRIDE;
  virtual int GetWidthNeededForText() const OVERRIDE;
  virtual void ExecuteTextCommand(int command_id) OVERRIDE;
  virtual bool HasTextBeingDragged() OVERRIDE;
  virtual gfx::Point GetContextMenuLocation() OVERRIDE;

  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual bool IsItemForCommandIdDynamic(int command_id) const OVERRIDE;
  virtual string16 GetLabelForCommandId(int command_id) const OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

  
  static const char kViewClassName[];

 protected:
  
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;

 private:
  friend class NativeTextfieldViewsTest;
  friend class TouchSelectionControllerImplTest;

  
  virtual void SetCompositionText(
      const ui::CompositionText& composition) OVERRIDE;
  virtual void ConfirmCompositionText() OVERRIDE;
  virtual void ClearCompositionText() OVERRIDE;
  virtual void InsertText(const string16& text) OVERRIDE;
  virtual void InsertChar(char16 ch, int flags) OVERRIDE;
  virtual gfx::NativeWindow GetAttachedWindow() const OVERRIDE;
  virtual ui::TextInputType GetTextInputType() const OVERRIDE;
  virtual ui::TextInputMode GetTextInputMode() const OVERRIDE;
  virtual bool CanComposeInline() const OVERRIDE;
  virtual gfx::Rect GetCaretBounds() const OVERRIDE;
  virtual bool GetCompositionCharacterBounds(uint32 index,
                                             gfx::Rect* rect) const OVERRIDE;
  virtual bool HasCompositionText() const OVERRIDE;
  virtual bool GetTextRange(gfx::Range* range) const OVERRIDE;
  virtual bool GetCompositionTextRange(gfx::Range* range) const OVERRIDE;
  virtual bool GetSelectionRange(gfx::Range* range) const OVERRIDE;
  virtual bool SetSelectionRange(const gfx::Range& range) OVERRIDE;
  virtual bool DeleteRange(const gfx::Range& range) OVERRIDE;
  virtual bool GetTextFromRange(const gfx::Range& range,
                                string16* text) const OVERRIDE;
  virtual void OnInputMethodChanged() OVERRIDE;
  virtual bool ChangeTextDirectionAndLayoutAlignment(
      base::i18n::TextDirection direction) OVERRIDE;
  virtual void ExtendSelectionAndDelete(size_t before, size_t after) OVERRIDE;
  virtual void EnsureCaretInRect(const gfx::Rect& rect) OVERRIDE;
  virtual void OnCandidateWindowShown() OVERRIDE;
  virtual void OnCandidateWindowUpdated() OVERRIDE;
  virtual void OnCandidateWindowHidden() OVERRIDE;

  
  virtual void OnCompositionTextConfirmedOrCleared() OVERRIDE;

  
  gfx::RenderText* GetRenderText() const;

  
  
  string16 GetTextForDisplay(const string16& text);

  
  void UpdateColorsFromTheme(const ui::NativeTheme* theme);

  
  void UpdateCursor();

  
  void RepaintCursor();

  
  void UpdateCursorBoundsAndTextOffset(size_t cursor_pos, bool insert_mode);

  void PaintTextAndCursor(gfx::Canvas* canvas);

  
  bool HandleKeyEvent(const ui::KeyEvent& key_event);

  
  bool MoveCursorTo(const gfx::Point& point, bool select);

  
  
  void PropagateTextChange();

  
  
  void UpdateAfterChange(bool text_changed, bool cursor_changed);

  
  void UpdateContextMenu();

  
  void OnTextInputTypeChanged();

  
  void OnCaretBoundsChanged();

  
  void OnBeforeUserAction();

  
  void OnAfterUserAction();

  
  bool Cut();

  
  bool Copy();

  
  
  bool Paste();

  
  void TrackMouseClicks(const ui::MouseEvent& event);

  
  void HandleMousePressEvent(const ui::MouseEvent& event);

  
  bool ImeEditingAllowed() const;

  
  
  bool ExceededDragThresholdFromLastClickLocation(const ui::MouseEvent& event);

  
  
  static bool ShouldInsertChar(char16 ch, int flags);

  void CreateTouchSelectionControllerAndNotifyIt();

  
  void PlatformGestureEventHandling(const ui::GestureEvent* event);

  
  
  void RevealObscuredChar(int index, const base::TimeDelta& duration);

  
  Textfield* textfield_;

  
  scoped_ptr<TextfieldViewsModel> model_;

  
  
  
  FocusableBorder* text_border_;

  
  bool is_cursor_visible_;

  
  bool is_drop_cursor_visible_;
  
  gfx::SelectionModel drop_cursor_position_;

  
  bool skip_input_method_cancel_composition_;

  
  bool initiating_drag_;

  
  base::WeakPtrFactory<NativeTextfieldViews> cursor_timer_;

  
  size_t aggregated_clicks_;
  base::TimeDelta last_click_time_;
  gfx::Point last_click_location_;
  gfx::Range double_click_word_;

  
  scoped_ptr<ui::SimpleMenuModel> context_menu_contents_;
  scoped_ptr<views::MenuModelAdapter> context_menu_delegate_;
  scoped_ptr<views::MenuRunner> context_menu_runner_;

  scoped_ptr<ui::TouchSelectionController> touch_selection_controller_;

  
  
  
  base::OneShotTimer<NativeTextfieldViews> obscured_reveal_timer_;

  DISALLOW_COPY_AND_ASSIGN(NativeTextfieldViews);
};

}  

#endif  
