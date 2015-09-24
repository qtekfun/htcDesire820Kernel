// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_SELECTED_KEYWORD_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_SELECTED_KEYWORD_VIEW_H_

#include <string>

#include "base/compiler_specific.h"
#include "chrome/browser/ui/views/location_bar/icon_label_bubble_view.h"
#include "ui/views/controls/label.h"

class Profile;
namespace gfx {
class FontList;
class Size;
}

class SelectedKeywordView : public IconLabelBubbleView {
 public:
  SelectedKeywordView(const gfx::FontList& font_list,
                      SkColor text_color,
                      SkColor parent_background_color,
                      Profile* profile);
  virtual ~SelectedKeywordView();

  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual gfx::Size GetMinimumSize() OVERRIDE;
  virtual void Layout() OVERRIDE;

  
  void SetKeyword(const base::string16& keyword);
  const base::string16& keyword() const { return keyword_; }

 private:
  
  
  
  base::string16 keyword_;

  
  
  
  
  views::Label full_label_;
  views::Label partial_label_;

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(SelectedKeywordView);
};

#endif  
