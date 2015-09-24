// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_KEYWORD_HINT_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_KEYWORD_HINT_VIEW_H_
#pragma once

#include <string>

#include "ui/gfx/size.h"
#include "views/view.h"

namespace gfx {
class Font;
}
class Profile;
namespace views {
class Label;
}

class KeywordHintView : public views::View {
 public:
  explicit KeywordHintView(Profile* profile);
  virtual ~KeywordHintView();

  void SetFont(const gfx::Font& font);

  void SetColor(const SkColor& color);

  void SetKeyword(const string16& keyword);
  string16 keyword() const { return keyword_; }

  virtual void OnPaint(gfx::Canvas* canvas);
  virtual gfx::Size GetPreferredSize();
  
  virtual gfx::Size GetMinimumSize();
  virtual void Layout();

  void set_profile(Profile* profile) { profile_ = profile; }

 private:
  views::Label* leading_label_;
  views::Label* trailing_label_;

  
  string16 keyword_;

  Profile* profile_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(KeywordHintView);
};

#endif  
