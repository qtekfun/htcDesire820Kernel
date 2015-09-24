// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_OMNIBOX_OMNIBOX_POPUP_MODEL_H_
#define CHROME_BROWSER_UI_OMNIBOX_OMNIBOX_POPUP_MODEL_H_

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "chrome/browser/autocomplete/autocomplete_controller.h"
#include "chrome/browser/autocomplete/autocomplete_result.h"
#include "chrome/browser/ui/omnibox/omnibox_edit_model.h"

class OmniboxPopupModelObserver;
class OmniboxPopupView;

namespace gfx {
class Image;
}

class OmniboxPopupModel {
 public:
  
  enum LineState {
    NORMAL = 0,
    KEYWORD
  };

  OmniboxPopupModel(OmniboxPopupView* popup_view, OmniboxEditModel* edit_model);
  ~OmniboxPopupModel();

  
  bool IsOpen() const;

  OmniboxPopupView* view() const { return view_; }

  
  AutocompleteController* autocomplete_controller() const {
    return edit_model_->autocomplete_controller();
  }

  const AutocompleteResult& result() const {
    return autocomplete_controller()->result();
  }

  size_t hovered_line() const { return hovered_line_; }

  
  
  void SetHoveredLine(size_t line);

  size_t selected_line() const { return selected_line_; }

  LineState selected_line_state() const { return selected_line_state_; }

  
  
  
  
  
  
  
  
  
  
  void SetSelectedLine(size_t line, bool reset_to_default, bool force);

  
  
  void ResetToDefaultMatch();

  
  
  
  
  
  void Move(int count);

  
  
  
  void SetSelectedLineState(LineState state);

  
  
  void TryDeletingCurrentItem();

  
  
  gfx::Image GetIconIfExtensionMatch(const AutocompleteMatch& match) const;

  
  const AutocompleteResult::Selection& manually_selected_match() const {
    return manually_selected_match_;
  }

  
  
  void OnResultChanged();

  
  void AddObserver(OmniboxPopupModelObserver* observer);
  void RemoveObserver(OmniboxPopupModelObserver* observer);

  
  
  static const size_t kNoMatch;

 private:
  OmniboxPopupView* view_;

  OmniboxEditModel* edit_model_;

  
  
  size_t hovered_line_;

  
  
  size_t selected_line_;

  
  
  
  LineState selected_line_state_;

  
  AutocompleteResult::Selection manually_selected_match_;

  
  ObserverList<OmniboxPopupModelObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(OmniboxPopupModel);
};

#endif  
