// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_KEYWORD_HINT_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_KEYWORD_HINT_VIEW_H_

#include <string>

#include "base/compiler_specific.h"
#include "ui/gfx/size.h"
#include "ui/views/view.h"

class Profile;

namespace gfx {
class FontList;
}

namespace views {
class ImageView;
class Label;
}

class KeywordHintView : public views::View {
 public:
  KeywordHintView(Profile* profile,
                  const gfx::FontList& font_list,
                  SkColor text_color,
                  SkColor background_color);
  virtual ~KeywordHintView();

  void SetKeyword(const base::string16& keyword);
  base::string16 keyword() const { return keyword_; }

  virtual gfx::Size GetPreferredSize() OVERRIDE;
  
  virtual gfx::Size GetMinimumSize() OVERRIDE;
  virtual void Layout() OVERRIDE;

 private:
  views::Label* CreateLabel(const gfx::FontList& font_list,
                            SkColor text_color,
                            SkColor background_color);

  Profile* profile_;
  views::Label* leading_label_;
  views::ImageView* tab_image_;
  views::Label* trailing_label_;
  base::string16 keyword_;

  DISALLOW_COPY_AND_ASSIGN(KeywordHintView);
};

#endif  
