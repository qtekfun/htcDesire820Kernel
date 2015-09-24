// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_EDIT_H_
#define CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_EDIT_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "chrome/browser/autocomplete/autocomplete_controller_delegate.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "chrome/common/instant_types.h"
#include "content/common/page_transition_types.h"
#include "googleurl/src/gurl.h"
#include "ui/gfx/native_widget_types.h"
#include "webkit/glue/window_open_disposition.h"

class AutocompleteController;
class AutocompleteEditController;
class AutocompleteEditModel;
class AutocompleteEditView;
class AutocompletePopupModel;
class AutocompleteResult;
class InstantController;
class Profile;
class SkBitmap;
class TabContentsWrapper;

namespace gfx {
class Rect;
}


class AutocompleteEditController {
 public:
  
  
  
  
  
  
  
  virtual void OnAutocompleteAccept(const GURL& url,
                                    WindowOpenDisposition disposition,
                                    PageTransition::Type transition,
                                    const GURL& alternate_nav_url) = 0;

  
  
  
  virtual void OnChanged() = 0;

  
  virtual void OnSelectionBoundsChanged() = 0;

  
  
  
  virtual void OnInputInProgress(bool in_progress) = 0;

  
  virtual void OnKillFocus() = 0;

  
  virtual void OnSetFocus() = 0;

  
  virtual SkBitmap GetFavicon() const = 0;

  
  virtual string16 GetTitle() const = 0;

  
  virtual InstantController* GetInstant() = 0;

  
  virtual TabContentsWrapper* GetTabContentsWrapper() const = 0;

 protected:
  virtual ~AutocompleteEditController();
};

class AutocompleteEditModel : public AutocompleteControllerDelegate {
 public:
  struct State {
    State(bool user_input_in_progress,
          const string16& user_text,
          const string16& keyword,
          bool is_keyword_hint);
    ~State();

    bool user_input_in_progress;
    const string16 user_text;
    const string16 keyword;
    const bool is_keyword_hint;
  };

  AutocompleteEditModel(AutocompleteEditView* view,
                        AutocompleteEditController* controller,
                        Profile* profile);
  ~AutocompleteEditModel();

  AutocompleteController* autocomplete_controller() const {
    return autocomplete_controller_.get();
  }

  void set_popup_model(AutocompletePopupModel* popup_model) {
    popup_ = popup_model;
  }

  
  
  AutocompletePopupModel* popup_model() const { return popup_; }

  
  void SetProfile(Profile* profile);

  Profile* profile() const { return profile_; }

  
  
  const State GetStateForTabSwitch();

  
  void RestoreState(const State& state);

  
  
  AutocompleteMatch CurrentMatch();

  
  
  void GetDataForURLExport(GURL* url, string16* title, SkBitmap* favicon);

  
  
  
  bool UseVerbatimInstant();

  
  
  
  
  string16 GetDesiredTLD() const;

  
  
  bool CurrentTextIsURL() const;

  
  AutocompleteMatch::Type CurrentTextType() const;

  
  
  
  
  
  
  void AdjustTextForCopy(int sel_min,
                         bool is_all_selected,
                         string16* text,
                         GURL* url,
                         bool* write_url);

  bool user_input_in_progress() const { return user_input_in_progress_; }

  
  
  void SetInputInProgress(bool in_progress);

  
  
  
  bool UpdatePermanentText(const string16& new_permanent_text);

  
  GURL PermanentURL();

  
  void SetUserText(const string16& text);

  
  
  
  void FinalizeInstantQuery(const string16& input_text,
                            const string16& suggest_text,
                            bool skip_inline_autocomplete);

  
  void SetSuggestedText(const string16& text,
                        InstantCompleteBehavior behavior);

  
  
  
  
  
  bool CommitSuggestedText(bool skip_inline_autocomplete);

  
  
  bool AcceptCurrentInstantPreview();

  
  
  void OnChanged();

  
  
  void Revert();

  
  void StartAutocomplete(bool has_selected_text,
                         bool prevent_inline_autocomplete) const;

  
  void StopAutocomplete();

  
  
  
  
  bool CanPasteAndGo(const string16& text) const;

  
  void PasteAndGo();

  
  const GURL& paste_and_go_url() const { return paste_and_go_url_; }

  
  
  bool is_paste_and_search() const {
    return (paste_and_go_transition_ != PageTransition::TYPED);
  }

  
  
  
  
  
  void AcceptInput(WindowOpenDisposition disposition,
                   bool for_drop);

  
  void OpenURL(const GURL& url,
               WindowOpenDisposition disposition,
               PageTransition::Type transition,
               const GURL& alternate_nav_url,
               size_t index,
               const string16& keyword);

  bool has_focus() const { return has_focus_; }

  
  
  const string16& keyword() const { return keyword_; }
  bool is_keyword_hint() const { return is_keyword_hint_; }

  
  
  bool AcceptKeyword();

  
  
  void ClearKeyword(const string16& visible_text);

  
  
  
  const AutocompleteResult& result() const;

  
  
  void OnSetFocus(bool control_down);

  
  void OnWillKillFocus(gfx::NativeView view_gaining_focus);

  
  void OnKillFocus();

  
  
  bool OnEscapeKeyPressed();

  
  
  void OnControlKeyChanged(bool pressed);

  
  void on_paste() { paste_state_ = PASTING; }

  
  
  void OnUpOrDownKeyPressed(int count);

  
  
  
  
  
  
  
  
  
  
  
  
  void OnPopupDataChanged(
      const string16& text,
      GURL* destination_for_temporary_text_change,
      const string16& keyword,
      bool is_keyword_hint);

  
  
  
  
  
  
  bool OnAfterPossibleChange(const string16& new_text,
                             size_t selection_start,
                             size_t selection_end,
                             bool selection_differs,
                             bool text_differs,
                             bool just_deleted_text,
                             bool allow_keyword_ui_change);

  
  void PopupBoundsChangedTo(const gfx::Rect& bounds);

#if defined(UNIT_TEST)
  InstantCompleteBehavior instant_complete_behavior() const {
    return instant_complete_behavior_;
  }
#endif

 private:
  enum PasteState {
    NONE,           
    PASTING,        
                    
                    
                    
                    
                    
    PASTED,         
  };

  enum ControlKeyState {
    UP,                   
    DOWN_WITHOUT_CHANGE,  
                          
                          
                          
                          
    DOWN_WITH_CHANGE,     
                          
                          
                          
                          
  };

  
  virtual void OnResultChanged(bool default_match_changed);

  
  
  
  
  bool query_in_progress() const;

  
  void InternalSetUserText(const string16& text);

  
  bool KeywordIsSelected() const;

  
  
  
  string16 DisplayTextFromUserText(const string16& text) const;
  string16 UserTextFromDisplayText(const string16& text) const;

  
  
  
  
  
  
  
  
  
  
  void InfoForCurrentSelection(AutocompleteMatch* match,
                               GURL* alternate_nav_url) const;

  
  
  void GetInfoForCurrentText(AutocompleteMatch* match,
                             GURL* alternate_nav_url) const;

  
  
  
  
  
  
  bool GetURLForText(const string16& text, GURL* url) const;

  
  
  void RevertTemporaryText(bool revert_popup);

  
  
  
  bool MaybeAcceptKeywordBySpace(const string16& old_user_text,
                                 const string16& new_user_text);

  
  
  
  
  
  bool ShouldAllowExactKeywordMatch(const string16& old_user_text,
                                    const string16& new_user_text,
                                    size_t caret_position);

  
  
  static bool IsSpaceCharForAcceptingKeyword(wchar_t c);

  scoped_ptr<AutocompleteController> autocomplete_controller_;

  AutocompleteEditView* view_;

  AutocompletePopupModel* popup_;

  AutocompleteEditController* controller_;

  
  bool has_focus_;

  
  string16 permanent_text_;

  
  
  
  
  
  bool user_input_in_progress_;

  
  
  string16 user_text_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  string16 url_for_remembered_user_selection_;

  
  
  
  
  
  
  
  bool just_deleted_text_;
  string16 inline_autocomplete_text_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool has_temporary_text_;
  GURL original_url_;

  
  
  
  PasteState paste_state_;

  
  
  
  ControlKeyState control_key_state_;

  
  
  
  
  string16 keyword_;

  
  
  
  bool is_keyword_hint_;

  
  mutable GURL paste_and_go_url_;
  mutable PageTransition::Type paste_and_go_transition_;
  mutable GURL paste_and_go_alternate_nav_url_;

  Profile* profile_;

  
  
  
  
  bool update_instant_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  bool allow_exact_keyword_match_;

  
  InstantCompleteBehavior instant_complete_behavior_;

  DISALLOW_COPY_AND_ASSIGN(AutocompleteEditModel);
};

#endif  
