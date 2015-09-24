// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_INFOBARS_TRANSLATE_INFOBAR_BASE_H_
#define CHROME_BROWSER_UI_VIEWS_INFOBARS_TRANSLATE_INFOBAR_BASE_H_

#include "base/compiler_specific.h"
#include "chrome/browser/ui/views/infobars/infobar_background.h"
#include "chrome/browser/ui/views/infobars/infobar_view.h"

class TranslateInfoBarDelegate;

namespace views {
class MenuButton;
}

class TranslateInfoBarBase : public InfoBarView {
 public:
  
  void UpdateLanguageButtonText(views::MenuButton* button,
                                const base::string16& text);

 protected:
  explicit TranslateInfoBarBase(scoped_ptr<TranslateInfoBarDelegate> delegate);
  virtual ~TranslateInfoBarBase();

  
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;

  
  TranslateInfoBarDelegate* GetDelegate();

  static const int kButtonInLabelSpacing;

 private:
  
  virtual void OnPaintBackground(gfx::Canvas* canvas) OVERRIDE;
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

  
  const views::Background& GetBackground();

  
  
  void FadeBackground(gfx::Canvas* canvas,
                      double animation_value,
                      const views::Background& background);

  InfoBarBackground error_background_;
  scoped_ptr<gfx::SlideAnimation> background_color_animation_;

  DISALLOW_COPY_AND_ASSIGN(TranslateInfoBarBase);
};

#endif  
