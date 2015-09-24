// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_EDIT_VIEW_MAC_H_
#define CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_EDIT_VIEW_MAC_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "chrome/browser/autocomplete/autocomplete_edit_view.h"
#include "chrome/browser/ui/cocoa/location_bar/autocomplete_text_field.h"

class AutocompleteEditController;
class AutocompletePopupViewMac;
class Profile;
class ToolbarModel;

namespace ui {
class Clipboard;
}


class AutocompleteEditViewMac : public AutocompleteEditView,
                                public AutocompleteTextFieldObserver {
 public:
  AutocompleteEditViewMac(AutocompleteEditController* controller,
                          ToolbarModel* toolbar_model,
                          Profile* profile,
                          CommandUpdater* command_updater,
                          AutocompleteTextField* field);
  virtual ~AutocompleteEditViewMac();

  
  virtual AutocompleteEditModel* model();
  virtual const AutocompleteEditModel* model() const;

  virtual void SaveStateToTab(TabContents* tab);
  virtual void Update(const TabContents* tab_for_state_restoring);

  virtual void OpenURL(const GURL& url,
                       WindowOpenDisposition disposition,
                       PageTransition::Type transition,
                       const GURL& alternate_nav_url,
                       size_t selected_line,
                       const string16& keyword);

  virtual string16 GetText() const;

  virtual bool IsEditingOrEmpty() const;
  virtual int GetIcon() const;

  virtual void SetUserText(const string16& text);
  virtual void SetUserText(const string16& text,
                           const string16& display_text,
                           bool update_popup);

  virtual void SetWindowTextAndCaretPos(const string16& text,
                                        size_t caret_pos);

  virtual void SetForcedQuery();

  virtual bool IsSelectAll();
  virtual bool DeleteAtEndPressed();
  virtual void GetSelectionBounds(string16::size_type* start,
                                  string16::size_type* end);

  virtual void SelectAll(bool reversed);
  virtual void RevertAll();
  virtual void UpdatePopup();
  virtual void ClosePopup();
  virtual void SetFocus();
  virtual void OnTemporaryTextMaybeChanged(const string16& display_text,
                                           bool save_original_selection);
  virtual bool OnInlineAutocompleteTextMaybeChanged(
      const string16& display_text, size_t user_text_length);
  virtual void OnStartingIME();
  virtual void OnRevertTemporaryText();
  virtual void OnBeforePossibleChange();
  virtual bool OnAfterPossibleChange();
  virtual gfx::NativeView GetNativeView() const;
  virtual CommandUpdater* GetCommandUpdater();
  virtual void SetInstantSuggestion(const string16& input,
                                    bool animate_to_complete);
  virtual string16 GetInstantSuggestion() const;
  virtual int TextWidth() const;
  virtual bool IsImeComposing() const;

  
  virtual NSRange SelectionRangeForProposedRange(NSRange proposed_range);
  virtual void OnControlKeyChanged(bool pressed);
  virtual bool CanCopy();
  virtual void CopyToPasteboard(NSPasteboard* pboard);
  virtual void OnPaste();
  virtual bool CanPasteAndGo();
  virtual int GetPasteActionStringId();
  virtual void OnPasteAndGo();
  virtual void OnFrameChanged();
  virtual void OnDidBeginEditing();
  virtual void OnBeforeChange();
  virtual void OnDidChange();
  virtual void OnDidEndEditing();
  virtual bool OnDoCommandBySelector(SEL cmd);
  virtual void OnSetFocus(bool control_down);
  virtual void OnKillFocus();

  
  void FocusLocation(bool select_all);

  
  
  static string16 GetClipboardText(ui::Clipboard* clipboard);

  
  
  static NSFont* GetFieldFont();

  
  
  static NSImage* ImageForResource(int resource_id);

 private:
  
  
  
  
  bool OnBackspacePressed();

  
  
  NSRange GetSelectedRange() const;

  
  
  NSRange GetMarkedRange() const;

  
  
  
  bool IsFirstResponder() const;

  
  
  void SetSelectedRange(const NSRange range);

  
  
  void SetText(const string16& display_text);

  
  
  void SetTextInternal(const string16& display_text);

  
  void SetTextAndSelectedRange(const string16& display_text,
                               const NSRange range);

  
  NSString* GetNonSuggestTextSubstring() const;

  
  NSString* GetSuggestTextSubstring() const;

  
  
  
  void EmphasizeURLComponents();

  
  
  void ApplyTextAttributes(const string16& display_text,
                           NSMutableAttributedString* as);

  
  
  NSUInteger GetTextLength() const;

  
  void PlaceCaretAt(NSUInteger pos);

  
  bool IsCaretAtEnd() const;

  scoped_ptr<AutocompleteEditModel> model_;
  scoped_ptr<AutocompletePopupViewMac> popup_view_;

  AutocompleteEditController* controller_;
  ToolbarModel* toolbar_model_;

  
  
  CommandUpdater* command_updater_;

  AutocompleteTextField* field_;  

  
  
  NSRange saved_temporary_selection_;

  
  
  NSRange selection_before_change_;
  string16 text_before_change_;
  NSRange marked_range_before_change_;

  
  
  size_t suggest_text_length_;

  
  bool delete_was_pressed_;

  
  bool delete_at_end_pressed_;

  
  CGFloat line_height_;

  DISALLOW_COPY_AND_ASSIGN(AutocompleteEditViewMac);
};

#endif  
