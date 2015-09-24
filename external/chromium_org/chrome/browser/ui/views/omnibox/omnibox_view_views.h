// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_OMNIBOX_OMNIBOX_VIEW_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_OMNIBOX_OMNIBOX_VIEW_VIEWS_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/omnibox/omnibox_view.h"
#include "chrome/browser/ui/toolbar/toolbar_model.h"
#include "ui/base/window_open_disposition.h"
#include "ui/gfx/range/range.h"
#include "ui/views/controls/textfield/textfield.h"
#include "ui/views/controls/textfield/textfield_controller.h"

#if defined(OS_CHROMEOS)
#include "chromeos/ime/input_method_manager.h"
#endif

class LocationBarView;
class OmniboxPopupView;
class Profile;

namespace ui {
class OSExchangeData;
}  

class OmniboxViewViews
    : public views::Textfield,
      public OmniboxView,
#if defined(OS_CHROMEOS)
      public
          chromeos::input_method::InputMethodManager::CandidateWindowObserver,
#endif
      public views::TextfieldController {
 public:
  
  static const char kViewClassName[];

  OmniboxViewViews(OmniboxEditController* controller,
                   Profile* profile,
                   CommandUpdater* command_updater,
                   bool popup_window_mode,
                   LocationBarView* location_bar,
                   const gfx::FontList& font_list);
  virtual ~OmniboxViewViews();

  
  void Init();

  
  void SetBaseColor();

  
  virtual const char* GetClassName() const OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual bool OnKeyReleased(const ui::KeyEvent& event) OVERRIDE;
  virtual bool SkipDefaultKeyEventProcessing(
      const ui::KeyEvent& event) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;

  
  virtual void SaveStateToTab(content::WebContents* tab) OVERRIDE;
  virtual void OnTabChanged(const content::WebContents* web_contents) OVERRIDE;
  virtual void Update() OVERRIDE;
  virtual base::string16 GetText() const OVERRIDE;
  virtual void SetUserText(const base::string16& text,
                           const base::string16& display_text,
                           bool update_popup) OVERRIDE;
  virtual void SetWindowTextAndCaretPos(const base::string16& text,
                                        size_t caret_pos,
                                        bool update_popup,
                                        bool notify_text_changed) OVERRIDE;
  virtual void SetForcedQuery() OVERRIDE;
  virtual bool IsSelectAll() const OVERRIDE;
  virtual bool DeleteAtEndPressed() OVERRIDE;
  virtual void GetSelectionBounds(
      base::string16::size_type* start,
      base::string16::size_type* end) const OVERRIDE;
  virtual void SelectAll(bool reversed) OVERRIDE;
  virtual void RevertAll() OVERRIDE;
  virtual void UpdatePopup() OVERRIDE;
  virtual void SetFocus() OVERRIDE;
  virtual void ApplyCaretVisibility() OVERRIDE;
  virtual void OnTemporaryTextMaybeChanged(
      const base::string16& display_text,
      bool save_original_selection,
      bool notify_text_changed) OVERRIDE;
  virtual bool OnInlineAutocompleteTextMaybeChanged(
      const base::string16& display_text, size_t user_text_length) OVERRIDE;
  virtual void OnInlineAutocompleteTextCleared() OVERRIDE;
  virtual void OnRevertTemporaryText() OVERRIDE;
  virtual void OnBeforePossibleChange() OVERRIDE;
  virtual bool OnAfterPossibleChange() OVERRIDE;
  virtual gfx::NativeView GetNativeView() const OVERRIDE;
  virtual gfx::NativeView GetRelativeWindowForPopup() const OVERRIDE;
  virtual void SetGrayTextAutocompletion(const base::string16& input) OVERRIDE;
  virtual base::string16 GetGrayTextAutocompletion() const OVERRIDE;
  virtual int GetTextWidth() const OVERRIDE;
  virtual int GetWidth() const OVERRIDE;
  virtual bool IsImeComposing() const OVERRIDE;
  virtual bool IsImeShowingPopup() const OVERRIDE;
  virtual int OnPerformDrop(const ui::DropTargetEvent& event) OVERRIDE;

  
  virtual void ContentsChanged(views::Textfield* sender,
                               const base::string16& new_contents) OVERRIDE;
  virtual bool HandleKeyEvent(views::Textfield* sender,
                              const ui::KeyEvent& key_event) OVERRIDE;
  virtual void OnBeforeUserAction(views::Textfield* sender) OVERRIDE;
  virtual void OnAfterUserAction(views::Textfield* sender) OVERRIDE;
  virtual void OnAfterCutOrCopy() OVERRIDE;
  virtual void OnWriteDragData(ui::OSExchangeData* data) OVERRIDE;
  virtual void OnGetDragOperationsForTextfield(int* drag_operations) OVERRIDE;
  virtual void AppendDropFormats(
      int* formats,
      std::set<ui::OSExchangeData::CustomFormat>* custom_formats) OVERRIDE;
  virtual int OnDrop(const ui::OSExchangeData& data) OVERRIDE;
  virtual void UpdateContextMenu(ui::SimpleMenuModel* menu_contents) OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool IsItemForCommandIdDynamic(int command_id) const OVERRIDE;
  virtual base::string16 GetLabelForCommandId(int command_id) const OVERRIDE;
  virtual bool HandlesCommand(int command_id) const OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

#if defined(OS_CHROMEOS)
  
  virtual void CandidateWindowOpened(
      chromeos::input_method::InputMethodManager* manager) OVERRIDE;
  virtual void CandidateWindowClosed(
      chromeos::input_method::InputMethodManager* manager) OVERRIDE;
#endif

 private:
  
  virtual int GetOmniboxTextLength() const OVERRIDE;

  
  virtual void EmphasizeURLComponents() OVERRIDE;

  
  void SetTextAndSelectedRange(const base::string16& text,
                               const gfx::Range& range);

  
  base::string16 GetSelectedText() const;

  
  
  
  
  
  
  void OnPaste();

  
  bool HandleEarlyTabActions(const ui::KeyEvent& event);

  
  
  bool popup_window_mode_;

  scoped_ptr<OmniboxPopupView> popup_view_;

  ToolbarModel::SecurityLevel security_level_;

  
  gfx::Range saved_temporary_selection_;

  
  
  gfx::Range saved_selection_for_focus_change_;

  
  base::string16 text_before_change_;
  gfx::Range sel_before_change_;
  bool ime_composing_before_change_;

  
  bool delete_at_end_pressed_;
  LocationBarView* location_bar_view_;

  
  
  
  bool ime_candidate_window_open_;

  
  
  
  
  bool select_all_on_mouse_release_;

  
  
  
  bool select_all_on_gesture_tap_;

  DISALLOW_COPY_AND_ASSIGN(OmniboxViewViews);
};

#endif  
