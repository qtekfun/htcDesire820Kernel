// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_SUGGESTED_TEXT_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_SUGGESTED_TEXT_VIEW_H_
#pragma once

#include "ui/base/animation/animation_delegate.h"
#include "views/controls/label.h"

class AutocompleteEditModel;

class SuggestedTextView : public views::Label,
                          public ui::AnimationDelegate {
 public:
  explicit SuggestedTextView(AutocompleteEditModel* edit_model);
  virtual ~SuggestedTextView();

  
  
  
  
  void StartAnimation();

  
  void StopAnimation();

  
  virtual void OnPaintBackground(gfx::Canvas* canvas);

  
  virtual void AnimationEnded(const ui::Animation* animation);
  virtual void AnimationProgressed(const ui::Animation* animation);
  virtual void AnimationCanceled(const ui::Animation* animation);

 private:
  
  ui::Animation* CreateAnimation();

  
  void UpdateBackgroundColor();

  AutocompleteEditModel* edit_model_;

  scoped_ptr<ui::Animation> animation_;

  SkColor bg_color_;

  DISALLOW_COPY_AND_ASSIGN(SuggestedTextView);
};

#endif  
