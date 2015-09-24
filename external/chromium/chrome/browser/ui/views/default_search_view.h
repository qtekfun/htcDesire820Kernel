// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_DEFAULT_SEARCH_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_DEFAULT_SEARCH_VIEW_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "content/browser/tab_contents/constrained_window.h"
#include "views/window/dialog_delegate.h"

#if defined(TOOLKIT_USES_GTK)
#include "chrome/browser/ui/gtk/constrained_window_gtk.h"
#endif

class PrefService;
class TabContents;
class TemplateURL;
class TemplateURLModel;

namespace gfx {
class Canvas;
}

namespace views {
class Button;
class ImageView;
class Label;
class View;
}

class DefaultSearchView
    : public views::View,
      public views::ButtonListener,
      public ConstrainedDialogDelegate {
 public:
  
  static void Show(TabContents* tab_contents,
                   TemplateURL* ,
                   TemplateURLModel* template_url_model);

  virtual ~DefaultSearchView();

 protected:
  
  
  virtual void OnPaint(gfx::Canvas* canvas);

  
  virtual void ButtonPressed(views::Button* sender, const views::Event& event);

  
  virtual std::wstring GetWindowTitle() const;
  virtual views::View* GetInitiallyFocusedView();
  virtual views::View* GetContentsView();
  virtual int GetDialogButtons() const;
  virtual bool Accept();

 private:
  
  DefaultSearchView(TabContents* tab_contents,
                    TemplateURL* proposed_default_turl,
                    TemplateURLModel* template_url_model);

  
  void SetupControls(PrefService* prefs);

  
  views::ImageView* background_image_;

  
  views::View* default_provider_button_;

  
  views::View* proposed_provider_button_;

  
  scoped_ptr<TemplateURL> proposed_turl_;

  TemplateURLModel* template_url_model_;

  DISALLOW_COPY_AND_ASSIGN(DefaultSearchView);
};

#endif  
