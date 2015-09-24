// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_INFOBARS_TRANSLATE_INFOBAR_BASE_GTK_H_
#define CHROME_BROWSER_UI_GTK_INFOBARS_TRANSLATE_INFOBAR_BASE_GTK_H_
#pragma once

#include "chrome/browser/ui/gtk/infobars/infobar_gtk.h"
#include "ui/base/animation/animation_delegate.h"

class MenuGtk;
class OptionsMenuModel;
class TranslateInfoBarDelegate;

class TranslateInfoBarBase : public InfoBar,
                             public ui::AnimationDelegate {
 public:
  explicit TranslateInfoBarBase(TranslateInfoBarDelegate* delegate);
  virtual ~TranslateInfoBarBase();

  
  
  virtual void Init();

  
  virtual void GetTopColor(InfoBarDelegate::Type type,
                           double* r, double* g, double *b);
  virtual void GetBottomColor(InfoBarDelegate::Type type,
                              double* r, double* g, double *b);

  
  virtual void AnimationProgressed(const ui::Animation* animation);

 protected:
  
  
  virtual bool ShowOptionsMenuButton() const;

  
  
  GtkWidget* CreateLabel(const std::string& text);

  
  
  
  
  
  
  GtkWidget* CreateLanguageCombobox(size_t selected_language,
                                    size_t exclude_language);

  
  
  static size_t GetLanguageComboboxActiveId(GtkComboBox* combo);

  
  TranslateInfoBarDelegate* GetDelegate() const;

 private:
  
  
  static GtkWidget* BuildOptionsMenuButton();

  
  scoped_ptr<OptionsMenuModel> options_menu_model_;
  scoped_ptr<MenuGtk> options_menu_menu_;

  CHROMEGTK_CALLBACK_0(TranslateInfoBarBase, void, OnOptionsClicked);

  
  
  
  double background_error_percent_;

  
  scoped_ptr<ui::SlideAnimation> background_color_animation_;

  DISALLOW_COPY_AND_ASSIGN(TranslateInfoBarBase);
};

#endif  
