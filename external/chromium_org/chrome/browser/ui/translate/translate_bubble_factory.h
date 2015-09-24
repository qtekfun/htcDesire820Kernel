// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TRANSLATE_TRANSLATE_BUBBLE_FACTORY_H_
#define CHROME_BROWSER_UI_TRANSLATE_TRANSLATE_BUBBLE_FACTORY_H_

#include "chrome/browser/ui/translate/translate_bubble_model.h"
#include "chrome/common/translate/translate_errors.h"

class BrowserWindow;

namespace content {
class WebContents;
}

class TranslateBubbleFactory {
 public:
  virtual ~TranslateBubbleFactory();

  
  
  static void Show(BrowserWindow* window,
                   content::WebContents* web_contents,
                   TranslateBubbleModel::ViewState view_state,
                   TranslateErrors::Type error_type);

  
  
  static void SetFactory(TranslateBubbleFactory* factory);

 protected:
  
  virtual void ShowImplementation(
      BrowserWindow* window,
      content::WebContents* web_contents,
      TranslateBubbleModel::ViewState view_state,
      TranslateErrors::Type error_type) = 0;

 private:
  static TranslateBubbleFactory* current_factory_;
};

#endif  
