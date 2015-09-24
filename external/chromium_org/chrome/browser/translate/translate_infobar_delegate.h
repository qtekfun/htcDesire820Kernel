// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TRANSLATE_TRANSLATE_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_TRANSLATE_TRANSLATE_INFOBAR_DELEGATE_H_

#include <string>
#include <utility>
#include <vector>

#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/infobars/infobar_delegate.h"
#include "chrome/browser/translate/translate_prefs.h"
#include "chrome/browser/translate/translate_ui_delegate.h"
#include "chrome/common/translate/translate_errors.h"
#include "components/translate/common/translate_constants.h"

class PrefService;

struct ShortcutConfiguration {
  int always_translate_min_count;
  int never_translate_min_count;
};

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

  virtual ~TranslateInfoBarDelegate();

  
  
  
  
  
  
  
  
  
  
  
  
  
  static void Create(bool replace_existing_infobar,
                     content::WebContents* web_contents,
                     Type infobar_type,
                     const std::string& original_language,
                     const std::string& target_language,
                     TranslateErrors::Type error_type,
                     PrefService* prefs,
                     const ShortcutConfiguration& shortcut_config);

  
  size_t num_languages() const { return ui_delegate_.GetNumberOfLanguages(); }

  
  std::string language_code_at(size_t index) const {
    return ui_delegate_.GetLanguageCodeAt(index);
  }

  
  base::string16 language_name_at(size_t index) const {
    return ui_delegate_.GetLanguageNameAt(index);
  }

  Type infobar_type() const { return infobar_type_; }

  TranslateErrors::Type error_type() const { return error_type_; }

  size_t original_language_index() const {
    return ui_delegate_.GetOriginalLanguageIndex();
  }
  void UpdateOriginalLanguageIndex(size_t language_index);

  size_t target_language_index() const {
    return ui_delegate_.GetTargetLanguageIndex();
  }
  void UpdateTargetLanguageIndex(size_t language_index);

  
  std::string original_language_code() const {
    return ui_delegate_.GetOriginalLanguageCode();
  }
  std::string target_language_code() const {
    return ui_delegate_.GetTargetLanguageCode();
  }

  
  
  bool is_error() const { return infobar_type_ == TRANSLATION_ERROR; }

  
  
  BackgroundAnimationType background_animation_type() const {
    return background_animation_;
  }

  virtual void Translate();
  virtual void RevertTranslation();
  void ReportLanguageDetectionError();

  
  
  virtual void TranslationDeclined();

  
  virtual bool IsTranslatableLanguageByPrefs();
  virtual void ToggleTranslatableLanguageByPrefs();
  virtual bool IsSiteBlacklisted();
  virtual void ToggleSiteBlacklist();
  virtual bool ShouldAlwaysTranslate();
  virtual void ToggleAlwaysTranslate();

  
  
  
  void AlwaysTranslatePageLanguage();
  void NeverTranslatePageLanguage();

  
  
  base::string16 GetMessageInfoBarText();
  base::string16 GetMessageInfoBarButtonText();
  void MessageInfoBarButtonPressed();
  bool ShouldShowMessageInfoBarButton();

  
  
  
  
  
  bool ShouldShowNeverTranslateShortcut();
  bool ShouldShowAlwaysTranslateShortcut();

  
  
  static base::string16 GetLanguageDisplayableName(
      const std::string& language_code);

  
  
  
  
  
  
  
  
  
  
  
  
  static void GetAfterTranslateStrings(std::vector<base::string16>* strings,
                                       bool* swap_languages,
                                       bool autodetermined_source_language);

 protected:
  TranslateInfoBarDelegate(content::WebContents* web_contents,
                           Type infobar_type,
                           TranslateInfoBarDelegate* old_delegate,
                           const std::string& original_language,
                           const std::string& target_language,
                           TranslateErrors::Type error_type,
                           PrefService* prefs,
                           ShortcutConfiguration shortcut_config);

 private:
  friend class TranslationInfoBarTest;
  typedef std::pair<std::string, base::string16> LanguageNamePair;

  
  static scoped_ptr<InfoBar> CreateInfoBar(
      scoped_ptr<TranslateInfoBarDelegate> delegate);

  
  virtual void InfoBarDismissed() OVERRIDE;
  virtual int GetIconID() const OVERRIDE;
  virtual InfoBarDelegate::Type GetInfoBarType() const OVERRIDE;
  virtual bool ShouldExpire(
       const content::LoadCommittedDetails& details) const OVERRIDE;
  virtual TranslateInfoBarDelegate* AsTranslateInfoBarDelegate() OVERRIDE;

  Type infobar_type_;

  
  
  BackgroundAnimationType background_animation_;

  TranslateUIDelegate ui_delegate_;

  
  TranslateErrors::Type error_type_;

  
  TranslatePrefs prefs_;

  
  ShortcutConfiguration shortcut_config_;
  DISALLOW_COPY_AND_ASSIGN(TranslateInfoBarDelegate);
};

#endif  
