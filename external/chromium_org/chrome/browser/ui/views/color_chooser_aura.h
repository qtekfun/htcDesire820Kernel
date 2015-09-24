// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_COLOR_CHOOSER_AURA_H_
#define CHROME_BROWSER_UI_VIEWS_COLOR_CHOOSER_AURA_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "content/public/browser/color_chooser.h"
#include "ui/views/color_chooser/color_chooser_listener.h"

namespace content {
class WebContents;
}

namespace views {
class ColorChooserView;
class Widget;
}

class ColorChooserAura : public content::ColorChooser,
                         public views::ColorChooserListener {
 public:
  static ColorChooserAura* Open(content::WebContents* web_contents,
                                SkColor initial_color);

 private:
  static ColorChooserAura* current_color_chooser_;

  ColorChooserAura(content::WebContents* web_contents, SkColor initial_color);

  
  virtual void End() OVERRIDE;
  virtual void SetSelectedColor(SkColor color) OVERRIDE;

  
  virtual void OnColorChosen(SkColor color) OVERRIDE;
  virtual void OnColorChooserDialogClosed() OVERRIDE;

  void DidEndColorChooser();

  
  
  views::ColorChooserView* view_;

  
  
  views::Widget* widget_;

  
  
  content::WebContents* web_contents_;

  DISALLOW_COPY_AND_ASSIGN(ColorChooserAura);
};

#endif  
