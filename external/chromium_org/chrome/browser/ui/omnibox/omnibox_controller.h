// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_OMNIBOX_OMNIBOX_CONTROLLER_H_
#define CHROME_BROWSER_UI_OMNIBOX_OMNIBOX_CONTROLLER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/autocomplete/autocomplete_controller.h"
#include "chrome/browser/autocomplete/autocomplete_controller_delegate.h"
#include "chrome/browser/autocomplete/autocomplete_input.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"

struct AutocompleteMatch;
class AutocompleteResult;
class GURL;
class InstantController;
class OmniboxEditModel;
class OmniboxPopupModel;
class Profile;

namespace gfx {
class Rect;
}

class OmniboxController : public AutocompleteControllerDelegate {
 public:
  OmniboxController(OmniboxEditModel* omnibox_edit_model,
                    Profile* profile);
  virtual ~OmniboxController();

  
  void StartAutocomplete(
      base::string16 user_text,
      size_t cursor_position,
      const GURL& current_url,
      AutocompleteInput::PageClassification current_page_classification,
      bool prevent_inline_autocomplete,
      bool prefer_keyword,
      bool allow_exact_keyword_match) const;

  
  virtual void OnResultChanged(bool default_match_changed) OVERRIDE;

  AutocompleteController* autocomplete_controller() {
    return autocomplete_controller_.get();
  }

  
  
  void InvalidateCurrentMatch();

  void set_popup_model(OmniboxPopupModel* popup_model) {
    popup_ = popup_model;
  }

  
  
  OmniboxPopupModel* popup_model() const { return popup_; }

  const AutocompleteMatch& current_match() const { return current_match_; }

  
  void ClearPopupKeywordMode() const;

  const AutocompleteResult& result() const {
    return autocomplete_controller_->result();
  }

  
  void DoPreconnect(const AutocompleteMatch& match);

 private:
  
  
  OmniboxEditModel* omnibox_edit_model_;

  Profile* profile_;

  OmniboxPopupModel* popup_;

  scoped_ptr<AutocompleteController> autocomplete_controller_;

  
  
  
  
  AutocompleteMatch current_match_;

  DISALLOW_COPY_AND_ASSIGN(OmniboxController);
};

#endif  
