// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TRANSLATE_TRANSLATE_BUBBLE_VIEW_STATE_TRANSITION_H_
#define CHROME_BROWSER_UI_TRANSLATE_TRANSLATE_BUBBLE_VIEW_STATE_TRANSITION_H_

#include "base/basictypes.h"
#include "chrome/browser/ui/translate/translate_bubble_model.h"

class TranslateBubbleViewStateTransition {
 public:
  explicit TranslateBubbleViewStateTransition(
      TranslateBubbleModel::ViewState view_state);

  TranslateBubbleModel::ViewState view_state() const { return view_state_; }

  
  void SetViewState(TranslateBubbleModel::ViewState view_state);

  
  void GoBackFromAdvanced();

 private:
  
  TranslateBubbleModel::ViewState view_state_;

  
  
  
  
  TranslateBubbleModel::ViewState view_state_before_advanced_view_;

  DISALLOW_COPY_AND_ASSIGN(TranslateBubbleViewStateTransition);
};

#endif  
