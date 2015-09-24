// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TRANSLATE_TRANSLATE_BUBBLE_MODEL_H_
#define CHROME_BROWSER_UI_TRANSLATE_TRANSLATE_BUBBLE_MODEL_H_

#include <string>

#include "base/strings/string16.h"
#include "chrome/common/translate/translate_errors.h"

class TranslateBubbleModel {
 public:
  enum ViewState {
    
    VIEW_STATE_BEFORE_TRANSLATE,

    
    VIEW_STATE_TRANSLATING,

    
    VIEW_STATE_AFTER_TRANSLATE,

    
    VIEW_STATE_ERROR,

    
    
    VIEW_STATE_ADVANCED,
  };

  virtual ~TranslateBubbleModel() {}

  
  virtual ViewState GetViewState() const = 0;

  
  virtual void SetViewState(ViewState view_state) = 0;

  
  virtual void ShowError(TranslateErrors::Type error_type) = 0;

  
  virtual void GoBackFromAdvanced() = 0;

  
  virtual int GetNumberOfLanguages() const = 0;

  
  virtual base::string16 GetLanguageNameAt(int index) const = 0;

  
  virtual int GetOriginalLanguageIndex() const = 0;

  
  virtual void UpdateOriginalLanguageIndex(int index) = 0;

  
  virtual int GetTargetLanguageIndex() const = 0;

  
  virtual void UpdateTargetLanguageIndex(int index) = 0;

  
  
  virtual void SetNeverTranslateLanguage(bool value) = 0;

  
  
  virtual void SetNeverTranslateSite(bool value) = 0;

  
  
  virtual bool ShouldAlwaysTranslate() const = 0;

  
  
  virtual void SetAlwaysTranslate(bool value) = 0;

  
  virtual void Translate() = 0;

  
  virtual void RevertTranslation() = 0;

  
  
  
  virtual void TranslationDeclined(bool explicitly_closed) = 0;

  
  
  virtual bool IsPageTranslatedInCurrentLanguages() const = 0;
};

#endif  
