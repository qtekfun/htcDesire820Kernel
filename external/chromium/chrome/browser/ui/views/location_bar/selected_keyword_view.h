// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_SELECTED_KEYWORD_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_SELECTED_KEYWORD_VIEW_H_
#pragma once

#include <string>

#include "chrome/browser/ui/views/location_bar/icon_label_bubble_view.h"
#include "views/controls/label.h"

class Profile;
namespace gfx {
class Font;
class Size;
}

class SelectedKeywordView : public IconLabelBubbleView {
 public:
  SelectedKeywordView(const int background_images[],
                      int contained_image,
                      const SkColor& color,
                      Profile* profile);
  virtual ~SelectedKeywordView();

  void SetFont(const gfx::Font& font);

  virtual gfx::Size GetPreferredSize();
  virtual gfx::Size GetMinimumSize();
  virtual void Layout();

  
  void SetKeyword(const string16& keyword);
  string16 keyword() const { return keyword_; }

  void set_profile(Profile* profile) { profile_ = profile; }

 private:
  
  
  
  string16 keyword_;

  
  
  
  
  views::Label full_label_;
  views::Label partial_label_;

  Profile* profile_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(SelectedKeywordView);
};

#endif  
