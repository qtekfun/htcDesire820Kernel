// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_OMNIBOX_OMNIBOX_VIEW_MAC_H_
#define CHROME_BROWSER_UI_COCOA_OMNIBOX_OMNIBOX_VIEW_MAC_H_

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/ui/cocoa/location_bar/autocomplete_text_field.h"
#include "chrome/browser/ui/omnibox/omnibox_view.h"

class OmniboxPopupView;

namespace ui {
class Clipboard;
}

class OmniboxViewMac : public OmniboxView,
                       public AutocompleteTextFieldObserver {
 public:
  OmniboxViewMac(OmniboxEditController* controller,
                 Profile* profile,
                 CommandUpdater* command_updater,
                 AutocompleteTextField* field);
  virtual ~OmniboxViewMac();

  
  virtual void SaveStateToTab(content::WebContents* tab) OVERRIDE;
  virtual void OnTabChanged(const content::WebContents* web_contents) OVERRIDE;
  virtual void Update() OVERRIDE;
  virtual base::string16 GetText() const OVERRIDE;
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
  virtual void CloseOmniboxPopup() OVERRIDE;
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

  
  virtual NSRange SelectionRangeForProposedRange(
      NSRange proposed_range) OVERRIDE;
  virtual void OnControlKeyChanged(bool pressed) OVERRIDE;
  virtual bool CanCopy() OVERRIDE;
  virtual void CopyToPasteboard(NSPasteboard* pboard) OVERRIDE;
  virtual bool ShouldEnableShowURL() OVERRIDE;
  virtual void ShowURL() OVERRIDE;
  virtual void OnPaste() OVERRIDE;
  virtual bool CanPasteAndGo() OVERRIDE;
  virtual int GetPasteActionStringId() OVERRIDE;
  virtual void OnPasteAndGo() OVERRIDE;
  virtual void OnFrameChanged() OVERRIDE;
  virtual void ClosePopup() OVERRIDE;
  virtual void OnDidBeginEditing() OVERRIDE;
  virtual void OnBeforeChange() OVERRIDE;
  virtual void OnDidChange() OVERRIDE;
  virtual void OnDidEndEditing() OVERRIDE;
  virtual bool OnDoCommandBySelector(SEL cmd) OVERRIDE;
  virtual void OnSetFocus(bool control_down) OVERRIDE;
  virtual void OnKillFocus() OVERRIDE;
  virtual void OnMouseDown(NSInteger button_number) OVERRIDE;
  virtual bool ShouldSelectAllOnMouseDown() OVERRIDE;

  
  void FocusLocation(bool select_all);

  
  
  static NSFont* GetFieldFont();

  
  
  static NSImage* ImageForResource(int resource_id);

  
  static NSColor* SuggestTextColor();

  AutocompleteTextField* field() const { return field_; }

 private:
  
  
  
  
  bool OnBackspacePressed();

  
  
  NSRange GetSelectedRange() const;

  
  
  NSRange GetMarkedRange() const;

  
  
  
  bool IsFirstResponder() const;

  
  
  void SetSelectedRange(const NSRange range);

  
  
  void SetText(const base::string16& display_text);

  
  
  void SetTextInternal(const base::string16& display_text);

  
  void SetTextAndSelectedRange(const base::string16& display_text,
                               const NSRange range);

  
  
  
  virtual void EmphasizeURLComponents() OVERRIDE;

  
  
  void ApplyTextAttributes(const base::string16& display_text,
                           NSMutableAttributedString* as);

  
  
  virtual int GetOmniboxTextLength() const OVERRIDE;
  NSUInteger GetTextLength() const;

  
  bool IsCaretAtEnd() const;

  scoped_ptr<OmniboxPopupView> popup_view_;

  AutocompleteTextField* field_;  

  
  
  NSRange saved_temporary_selection_;

  
  
  NSRange selection_before_change_;
  base::string16 text_before_change_;
  NSRange marked_range_before_change_;

  
  bool delete_was_pressed_;

  
  bool delete_at_end_pressed_;

  base::string16 suggest_text_;

  DISALLOW_COPY_AND_ASSIGN(OmniboxViewMac);
};

#endif  
