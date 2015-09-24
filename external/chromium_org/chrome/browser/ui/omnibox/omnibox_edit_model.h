// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_OMNIBOX_OMNIBOX_EDIT_MODEL_H_
#define CHROME_BROWSER_UI_OMNIBOX_OMNIBOX_EDIT_MODEL_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "chrome/browser/autocomplete/autocomplete_controller_delegate.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "chrome/browser/ui/omnibox/omnibox_controller.h"
#include "chrome/common/omnibox_focus_state.h"
#include "content/public/common/page_transition_types.h"
#include "ui/base/window_open_disposition.h"
#include "ui/gfx/native_widget_types.h"
#include "url/gurl.h"

class AutocompleteController;
class AutocompleteResult;
class OmniboxCurrentPageDelegate;
class OmniboxEditController;
class OmniboxPopupModel;
class OmniboxView;
class Profile;

namespace gfx {
class Image;
class Rect;
}

enum EnteredKeywordModeMethod {
  ENTERED_KEYWORD_MODE_VIA_TAB = 0,
  ENTERED_KEYWORD_MODE_VIA_SPACE_AT_END = 1,
  ENTERED_KEYWORD_MODE_VIA_SPACE_IN_MIDDLE = 2,
  ENTERED_KEYWORD_MODE_NUM_ITEMS
};

class OmniboxEditModel {
 public:
  
  
  enum FocusSource {
    INVALID = 0,
    OMNIBOX = 1,
    FAKEBOX = 2
  };

  struct State {
    State(bool user_input_in_progress,
          const base::string16& user_text,
          const base::string16& gray_text,
          const base::string16& keyword,
          bool is_keyword_hint,
          bool url_replacement_enabled,
          OmniboxFocusState focus_state,
          FocusSource focus_source);
    ~State();

    bool user_input_in_progress;
    const base::string16 user_text;
    const base::string16 gray_text;
    const base::string16 keyword;
    const bool is_keyword_hint;
    bool url_replacement_enabled;
    OmniboxFocusState focus_state;
    FocusSource focus_source;
  };

  OmniboxEditModel(OmniboxView* view,
                   OmniboxEditController* controller,
                   Profile* profile);
  virtual ~OmniboxEditModel();

  
  
  AutocompleteController* autocomplete_controller() const {
    return omnibox_controller_->autocomplete_controller();
  }

  void set_popup_model(OmniboxPopupModel* popup_model) {
    omnibox_controller_->set_popup_model(popup_model);
  }

  
  
  
  OmniboxPopupModel* popup_model() const {
    return omnibox_controller_->popup_model();
  }

  OmniboxEditController* controller() const { return controller_; }

  Profile* profile() const { return profile_; }

  
  
  const State GetStateForTabSwitch();

  
  
  void RestoreState(const State* state);

  
  
  
  
  AutocompleteMatch CurrentMatch(GURL* alternate_nav_url) const;

  
  
  void GetDataForURLExport(GURL* url,
                           base::string16* title,
                           gfx::Image* favicon);

  
  
  bool CurrentTextIsURL() const;

  
  AutocompleteMatch::Type CurrentTextType() const;

  
  
  
  
  
  
  void AdjustTextForCopy(int sel_min,
                         bool is_all_selected,
                         base::string16* text,
                         GURL* url,
                         bool* write_url);

  bool user_input_in_progress() const { return user_input_in_progress_; }

  
  
  void SetInputInProgress(bool in_progress);

  
  
  
  
  bool UpdatePermanentText();

  
  GURL PermanentURL();

  
  void SetUserText(const base::string16& text);

  
  
  
  bool CommitSuggestedText();

  
  
  void OnChanged();

  
  
  void Revert();

  
  void StartAutocomplete(bool has_selected_text,
                         bool prevent_inline_autocomplete) const;

  
  void StopAutocomplete();

  
  bool CanPasteAndGo(const base::string16& text) const;

  
  void PasteAndGo(const base::string16& text);

  
  
  bool IsPasteAndSearch(const base::string16& text) const;

  
  
  
  
  
  void AcceptInput(WindowOpenDisposition disposition,
                   bool for_drop);

  
  
  
  
  
  
  
  
  
  
  
  void OpenMatch(AutocompleteMatch match,
                 WindowOpenDisposition disposition,
                 const GURL& alternate_nav_url,
                 size_t index);

  OmniboxFocusState focus_state() const { return focus_state_; }
  bool has_focus() const { return focus_state_ != OMNIBOX_FOCUS_NONE; }
  bool is_caret_visible() const {
    return focus_state_ == OMNIBOX_FOCUS_VISIBLE;
  }

  
  
  const base::string16& keyword() const { return keyword_; }
  bool is_keyword_hint() const { return is_keyword_hint_; }

  
  
  
  
  bool AcceptKeyword(EnteredKeywordModeMethod entered_method);

  
  void AcceptTemporaryTextAsUserText();

  
  
  void ClearKeyword(const base::string16& visible_text);

  
  
  
  const AutocompleteResult& result() const {
    return omnibox_controller_->result();
  }

  
  
  void OnSetFocus(bool control_down);

  
  
  
  
  
  
  
  
  
  
  void SetCaretVisibility(bool visible);

  
  void OnWillKillFocus(gfx::NativeView view_gaining_focus);

  
  void OnKillFocus();

  
  
  bool OnEscapeKeyPressed();

  
  
  void OnControlKeyChanged(bool pressed);

  
  void OnPaste();

  
  bool is_pasting() const { return paste_state_ == PASTING; }

  
  bool in_revert() const { return in_revert_; }

  
  
  virtual void OnUpOrDownKeyPressed(int count);

  
  
  
  
  
  
  
  
  
  
  
  
  void OnPopupDataChanged(
      const base::string16& text,
      GURL* destination_for_temporary_text_change,
      const base::string16& keyword,
      bool is_keyword_hint);

  
  
  
  
  
  
  
  
  
  bool OnAfterPossibleChange(const base::string16& old_text,
                             const base::string16& new_text,
                             size_t selection_start,
                             size_t selection_end,
                             bool selection_differs,
                             bool text_differs,
                             bool just_deleted_text,
                             bool allow_keyword_ui_change);

  
  void OnCurrentMatchChanged();

  
  
  
  
  InstantController* GetInstantController() const;

  
  static const char kCutOrCopyAllTextHistogram[];

 private:
  friend class OmniboxControllerTest;

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

  
  
  
  
  bool query_in_progress() const;

  
  void InternalSetUserText(const base::string16& text);

  
  bool KeywordIsSelected() const;

  
  void ClearPopupKeywordMode() const;

  
  
  
  base::string16 DisplayTextFromUserText(const base::string16& text) const;
  base::string16 UserTextFromDisplayText(const base::string16& text) const;

  
  
  
  void GetInfoForCurrentText(AutocompleteMatch* match,
                             GURL* alternate_nav_url) const;

  
  
  void RevertTemporaryText(bool revert_popup);

  
  
  
  
  
  
  bool MaybeAcceptKeywordBySpace(const base::string16& new_text);

  
  
  bool CreatedKeywordSearchByInsertingSpaceInMiddle(
      const base::string16& old_text,
      const base::string16& new_text,
      size_t caret_position) const;

  
  
  static bool IsSpaceCharForAcceptingKeyword(wchar_t c);

  
  
  
  
  AutocompleteInput::PageClassification ClassifyPage() const;

  
  
  void ClassifyStringForPasteAndGo(const base::string16& text,
                                   AutocompleteMatch* match,
                                   GURL* alternate_nav_url) const;

  
  
  
  
  void SetFocusState(OmniboxFocusState state, OmniboxFocusChangeReason reason);

  scoped_ptr<OmniboxController> omnibox_controller_;

  OmniboxView* view_;

  OmniboxEditController* controller_;

  scoped_ptr<OmniboxCurrentPageDelegate> delegate_;

  OmniboxFocusState focus_state_;

  
  
  
  FocusSource focus_source_;

  
  base::string16 permanent_text_;

  
  
  
  
  
  bool user_input_in_progress_;

  
  
  base::string16 user_text_;

  
  base::TimeTicks last_omnibox_focus_;

  
  
  
  
  bool user_input_since_focus_;

  
  
  base::TimeTicks time_user_first_modified_omnibox_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  base::string16 url_for_remembered_user_selection_;

  
  
  
  
  
  
  
  bool just_deleted_text_;
  base::string16 inline_autocomplete_text_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool has_temporary_text_;
  GURL original_url_;

  
  
  
  PasteState paste_state_;

  
  
  
  ControlKeyState control_key_state_;

  
  
  
  
  base::string16 keyword_;

  
  
  
  bool is_keyword_hint_;

  Profile* profile_;

  
  
  bool in_revert_;

  
  
  
  
  
  bool allow_exact_keyword_match_;

  DISALLOW_COPY_AND_ASSIGN(OmniboxEditModel);
};

#endif  
