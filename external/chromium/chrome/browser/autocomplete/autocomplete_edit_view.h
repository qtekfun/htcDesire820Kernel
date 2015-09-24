// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_EDIT_VIEW_H_
#define CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_EDIT_VIEW_H_
#pragma once

#include <string>

#include "base/string16.h"
#include "content/common/page_transition_types.h"
#include "ui/gfx/native_widget_types.h"
#include "webkit/glue/window_open_disposition.h"

class AutocompleteEditModel;
class CommandUpdater;
class GURL;
class TabContents;

#if defined(TOOLKIT_VIEWS)
namespace views {
class DropTargetEvent;
class View;
}  
#endif

class AutocompleteEditView {
 public:
  
  virtual AutocompleteEditModel* model() = 0;
  virtual const AutocompleteEditModel* model() const = 0;

  
  
  virtual void SaveStateToTab(TabContents* tab) = 0;

  
  
  
  virtual void Update(const TabContents* tab_for_state_restoring) = 0;

  
  
  
  
  
  
  
  
  
  
  virtual void OpenURL(const GURL& url,
                       WindowOpenDisposition disposition,
                       PageTransition::Type transition,
                       const GURL& alternate_nav_url,
                       size_t selected_line,
                       const string16& keyword) = 0;

  
  
  
  virtual string16 GetText() const = 0;

  
  
  virtual bool IsEditingOrEmpty() const = 0;

  
  virtual int GetIcon() const = 0;

  
  
  
  virtual void SetUserText(const string16& text) = 0;
  virtual void SetUserText(const string16& text,
                           const string16& display_text,
                           bool update_popup) = 0;

  
  virtual void SetWindowTextAndCaretPos(const string16& text,
                                        size_t caret_pos) = 0;

  
  
  
  
  
  
  
  virtual void SetForcedQuery() = 0;

  
  virtual bool IsSelectAll() = 0;

  
  virtual bool DeleteAtEndPressed() = 0;

  
  
  
  
  virtual void GetSelectionBounds(string16::size_type* start,
                                  string16::size_type* end) = 0;

  
  
  virtual void SelectAll(bool reversed) = 0;

  
  
  virtual void RevertAll() = 0;

  
  
  virtual void UpdatePopup() = 0;

  
  virtual void ClosePopup() = 0;

  
  virtual void SetFocus() = 0;

  
  
  
  
  virtual void OnTemporaryTextMaybeChanged(const string16& display_text,
                                           bool save_original_selection) = 0;

  
  
  
  
  virtual bool OnInlineAutocompleteTextMaybeChanged(
      const string16& display_text, size_t user_text_length) = 0;

  
  
  virtual void OnRevertTemporaryText() = 0;

  
  
  
  virtual void OnBeforePossibleChange() = 0;
  
  
  virtual bool OnAfterPossibleChange() = 0;

  
  virtual gfx::NativeView GetNativeView() const = 0;

  
  virtual CommandUpdater* GetCommandUpdater() = 0;

  
  
  virtual void SetInstantSuggestion(const string16& input,
                                    bool animate_to_complete) = 0;

  
  virtual string16 GetInstantSuggestion() const = 0;

  
  
  virtual int TextWidth() const = 0;

  
  virtual bool IsImeComposing() const = 0;

#if defined(TOOLKIT_VIEWS)
  
  
  virtual views::View* AddToView(views::View* parent) = 0;

  
  virtual int OnPerformDrop(const views::DropTargetEvent& event) = 0;
#endif

  virtual ~AutocompleteEditView() {}
};

#endif  
