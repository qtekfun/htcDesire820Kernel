// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_INFOBARS_TRANSLATE_INFOBAR_BASE_GTK_H_
#define CHROME_BROWSER_UI_GTK_INFOBARS_TRANSLATE_INFOBAR_BASE_GTK_H_

#include "base/compiler_specific.h"
#include "chrome/browser/ui/gtk/infobars/infobar_gtk.h"
#include "ui/gfx/animation/animation_delegate.h"

class TranslateInfoBarDelegate;

class TranslateInfoBarBase : public InfoBarGtk {
 protected:
  explicit TranslateInfoBarBase(scoped_ptr<TranslateInfoBarDelegate> delegate);
  virtual ~TranslateInfoBarBase();

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual void PlatformSpecificSetOwner() OVERRIDE;
  virtual void GetTopColor(InfoBarDelegate::Type type,
                           double* r, double* g, double* b) OVERRIDE;
  virtual void GetBottomColor(InfoBarDelegate::Type type,
                              double* r, double* g, double* b) OVERRIDE;

  
  
  virtual bool ShowOptionsMenuButton() const;

  
  
  
  
  
  
  GtkWidget* CreateLanguageCombobox(size_t selected_language,
                                    size_t exclude_language);

  
  
  static size_t GetLanguageComboboxActiveId(GtkComboBox* combo);

  
  TranslateInfoBarDelegate* GetDelegate();

 private:
  
  
  
  enum {
    LANGUAGE_COMBO_COLUMN_ID,
    LANGUAGE_COMBO_COLUMN_NAME,
    LANGUAGE_COMBO_COLUMN_COUNT
  };

  CHROMEGTK_CALLBACK_0(TranslateInfoBarBase, void, OnOptionsClicked);

  
  
  
  double background_error_percent_;

  
  scoped_ptr<gfx::SlideAnimation> background_color_animation_;

  
  scoped_ptr<ui::MenuModel> menu_model_;

  DISALLOW_COPY_AND_ASSIGN(TranslateInfoBarBase);
};

#endif  
