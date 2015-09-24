// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TRANSLATE_TRANSLATE_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_TRANSLATE_TRANSLATE_INFOBAR_DELEGATE_H_
#pragma once

#include <string>
#include <vector>

#include "chrome/browser/tab_contents/confirm_infobar_delegate.h"
#include "chrome/browser/translate/translate_prefs.h"
#include "chrome/common/translate_errors.h"

class SkBitmap;
class TranslateInfoBarView;

class TranslateInfoBarDelegate : public InfoBarDelegate {
 public:
  
  enum Type {
    BEFORE_TRANSLATE,
    TRANSLATING,
    AFTER_TRANSLATE,
    TRANSLATION_ERROR
  };

  
  enum BackgroundAnimationType {
    NONE,
    NORMAL_TO_ERROR,
    ERROR_TO_NORMAL
  };

  static const size_t kNoIndex;

  
  
  
  
  
  static TranslateInfoBarDelegate* CreateDelegate(
      Type infobar_type,
      TabContents* tab_contents,
      const std::string& original_language,
      const std::string& target_language);

  
  static TranslateInfoBarDelegate* CreateErrorDelegate(
      TranslateErrors::Type error_type,
      TabContents* tab_contents,
      const std::string& original_language,
      const std::string& target_language);

  virtual ~TranslateInfoBarDelegate();

  
  size_t GetLanguageCount() const { return languages_.size(); }

  
  std::string GetLanguageCodeAt(size_t index) const;

  
  string16 GetLanguageDisplayableNameAt(size_t index) const;

  TabContents* tab_contents() const { return tab_contents_; }

  Type type() const { return type_; }

  TranslateErrors::Type error() const { return error_; }

  size_t original_language_index() const { return original_language_index_; }
  size_t target_language_index() const { return target_language_index_; }

  
  std::string GetOriginalLanguageCode() const;
  std::string GetTargetLanguageCode() const;

  
  
  virtual void SetOriginalLanguage(size_t language_index);
  virtual void SetTargetLanguage(size_t language_index);

  
  
  bool IsError() const { return type_ == TRANSLATION_ERROR; }

  
  
  BackgroundAnimationType background_animation_type() const {
    return background_animation_;
  }

  virtual void Translate();
  virtual void RevertTranslation();
  virtual void ReportLanguageDetectionError();

  
  
  void TranslationDeclined();

  
  virtual bool IsLanguageBlacklisted();
  virtual void ToggleLanguageBlacklist();
  virtual bool IsSiteBlacklisted();
  virtual void ToggleSiteBlacklist();
  virtual bool ShouldAlwaysTranslate();
  virtual void ToggleAlwaysTranslate();

  
  
  
  virtual void AlwaysTranslatePageLanguage();
  virtual void NeverTranslatePageLanguage();

  
  
  string16 GetMessageInfoBarText();
  string16 GetMessageInfoBarButtonText();
  void MessageInfoBarButtonPressed();
  bool ShouldShowMessageInfoBarButton();

  
  
  
  bool ShouldShowNeverTranslateButton();
  bool ShouldShowAlwaysTranslateButton();

  
  
  
  void UpdateBackgroundAnimation(TranslateInfoBarDelegate* previous_infobar);

  
  
  static string16 GetLanguageDisplayableName(const std::string& language_code);

  
  
  
  
  
  
  
  static void GetAfterTranslateStrings(std::vector<string16>* strings,
                                       bool* swap_languages);

 protected:
  
  TranslateInfoBarDelegate(Type infobar_type,
                           TranslateErrors::Type error,
                           TabContents* tab_contents,
                           const std::string& original_language,
                           const std::string& target_language);
  Type type_;

 private:
  typedef std::pair<std::string, string16> LanguageNamePair;

  
  virtual InfoBar* CreateInfoBar();
  virtual void InfoBarDismissed();
  virtual void InfoBarClosed();
  virtual SkBitmap* GetIcon() const;
  virtual InfoBarDelegate::Type GetInfoBarType() const;
  virtual TranslateInfoBarDelegate* AsTranslateInfoBarDelegate();

  
  
  std::string GetPageHost();

  
  
  BackgroundAnimationType background_animation_;

  TabContents* tab_contents_;

  
  
  
  
  std::vector<LanguageNamePair> languages_;

  
  size_t original_language_index_;

  
  
  
  
  
  size_t initial_original_language_index_;

  
  size_t target_language_index_;

  
  TranslateErrors::Type error_;

  
  TranslateInfoBarView* infobar_view_;

  
  TranslatePrefs prefs_;

  DISALLOW_COPY_AND_ASSIGN(TranslateInfoBarDelegate);
};

#endif  
