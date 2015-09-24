// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_FIRST_RUN_SEARCH_ENGINE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_FIRST_RUN_SEARCH_ENGINE_VIEW_H_
#pragma once

#include <vector>

#include "chrome/browser/search_engines/template_url_model_observer.h"
#include "ui/gfx/size.h"
#include "views/controls/button/native_button.h"
#include "views/view.h"
#include "views/window/window_delegate.h"

namespace views {
class ButtonListener;
class ImageView;
class Label;
class Separator;
class Window;
}

class Profile;
class TemplateURL;
class TemplateURLModel;

class SearchEngineChoice : public views::NativeButton {
 public:
  
  
  
  
  SearchEngineChoice(views::ButtonListener* listener,
                     const TemplateURL* search_engine,
                     bool use_small_logos);

  virtual ~SearchEngineChoice() {}

  
  
  views::View* GetView() { return choice_view_; }
  int GetChoiceViewWidth();
  int GetChoiceViewHeight();

  
  
  void SetChoiceViewBounds(int x, int y, int width, int height);

  
  const TemplateURL* GetSearchEngine() { return search_engine_; }

  
  void set_slot(int slot) { slot_ = slot; }
  int slot() const { return slot_; }

 private:
  
  
  views::View* choice_view_;

  
  bool is_image_label_;

  
  const TemplateURL* search_engine_;

  
  int slot_;

  DISALLOW_COPY_AND_ASSIGN(SearchEngineChoice);
};

class FirstRunSearchEngineView
    : public views::View,
      public views::ButtonListener,
      public views::WindowDelegate,
      public TemplateURLModelObserver {
 public:
  
  
  FirstRunSearchEngineView(Profile* profile, bool randomize);

  virtual ~FirstRunSearchEngineView();

  bool IsAlwaysOnTop() const { return true; }
  bool HasAlwaysOnTopMenu() const { return false; }

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual std::wstring GetWindowTitle() const OVERRIDE;
  views::View* GetContentsView() OVERRIDE { return this; }
  bool CanResize() const OVERRIDE{ return false; }
  bool CanMaximize() const OVERRIDE { return false; }

  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event)
      OVERRIDE;

  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  
  
  
  virtual void OnTemplateURLModelChanged() OVERRIDE;

 private:
  
  void SetupControls();

  
  TemplateURLModel* search_engines_model_;

  
  std::vector<SearchEngineChoice*> search_engine_choices_;

  
  bool randomize_;

  
  Profile* profile_;

  bool text_direction_is_rtl_;

  
  views::ImageView* background_image_;

  
  views::Label* title_label_;
  views::Label* text_label_;

  DISALLOW_COPY_AND_ASSIGN(FirstRunSearchEngineView);
};

#endif  
