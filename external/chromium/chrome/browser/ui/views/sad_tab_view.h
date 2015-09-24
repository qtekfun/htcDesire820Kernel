// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_SAD_TAB_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_SAD_TAB_VIEW_H_
#pragma once

#include "base/basictypes.h"
#include "ui/gfx/font.h"
#include "views/controls/link.h"
#include "views/view.h"

class SkBitmap;
class TabContents;

class SadTabView : public views::View,
                   public views::LinkController {
 public:
  enum Kind {
    CRASHED,  
    KILLED    
  };

  explicit SadTabView(TabContents* tab_contents, Kind kind);
  virtual ~SadTabView();

  
  virtual void OnPaint(gfx::Canvas* canvas);
  virtual void Layout();

  
  virtual void LinkActivated(views::Link* source, int event_flags);

 private:
  static void InitClass(Kind kind);

  
  static SkBitmap* sad_tab_bitmap_;
  static gfx::Font* title_font_;
  static gfx::Font* message_font_;
  static std::wstring title_;
  static std::wstring message_;
  static int title_width_;

  TabContents* tab_contents_;
  views::Link* learn_more_link_;

  
  
  gfx::Rect icon_bounds_;
  gfx::Rect title_bounds_;
  gfx::Rect message_bounds_;
  gfx::Rect link_bounds_;

  Kind kind_;

  DISALLOW_COPY_AND_ASSIGN(SadTabView);
};

#endif  
