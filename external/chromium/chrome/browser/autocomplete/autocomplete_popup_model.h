// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_POPUP_MODEL_H_
#define CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_POPUP_MODEL_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/autocomplete/autocomplete.h"
#include "chrome/browser/autocomplete/autocomplete_edit.h"

class AutocompleteEditView;
class AutocompletePopupView;
class Profile;
class SkBitmap;

class AutocompletePopupModel {
 public:
  AutocompletePopupModel(AutocompletePopupView* popup_view,
                         AutocompleteEditModel* edit_model,
                         Profile* profile);
  ~AutocompletePopupModel();

  
  void set_profile(Profile* profile) { profile_ = profile; }

  
  Profile* profile() const { return profile_; }

  
  bool IsOpen() const;

  AutocompletePopupView* view() const { return view_; }

  
  AutocompleteController* autocomplete_controller() const {
    return edit_model_->autocomplete_controller();
  }

  const AutocompleteResult& result() const {
    return autocomplete_controller()->result();
  }

  size_t hovered_line() const {
    return hovered_line_;
  }

  
  
  void SetHoveredLine(size_t line);

  size_t selected_line() const {
    return selected_line_;
  }

  
  
  
  
  
  
  
  
  
  
  void SetSelectedLine(size_t line, bool reset_to_default, bool force);

  
  
  void ResetToDefaultMatch();

  
  
  
  bool GetKeywordForMatch(const AutocompleteMatch& match,
                          string16* keyword) const;

  
  
  
  
  
  bool GetKeywordForText(const string16& text, string16* keyword) const;

  
  
  
  
  
  void Move(int count);

  
  
  void TryDeletingCurrentItem();

  
  
  const SkBitmap* GetIconIfExtensionMatch(const AutocompleteMatch& match) const;

  
  const AutocompleteResult::Selection& manually_selected_match() const {
    return manually_selected_match_;
  }

  
  
  void OnResultChanged();

  
  
  static const size_t kNoMatch = -1;

 private:
  AutocompletePopupView* view_;

  AutocompleteEditModel* edit_model_;

  
  Profile* profile_;

  
  
  size_t hovered_line_;

  
  
  size_t selected_line_;

  
  AutocompleteResult::Selection manually_selected_match_;

  DISALLOW_COPY_AND_ASSIGN(AutocompletePopupModel);
};

#endif  
