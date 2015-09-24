// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TRANSLATE_TRANSLATE_UI_DELEGATE_H_
#define CHROME_BROWSER_TRANSLATE_TRANSLATE_UI_DELEGATE_H_

#include <string>
#include <vector>

#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/common/chrome_constants.h"
#include "chrome/common/translate/translate_errors.h"

class TranslatePrefs;

namespace content {
class WebContents;
}  

class TranslateUIDelegate {
 public:
  enum {
    NO_INDEX = -1,
  };

  TranslateUIDelegate(content::WebContents* web_contents,
                      const std::string& original_language,
                      const std::string& target_language);
  virtual ~TranslateUIDelegate();

  content::WebContents* web_contents() { return web_contents_; }

  
  void OnErrorShown(TranslateErrors::Type error_type);

  
  size_t GetNumberOfLanguages() const;

  
  size_t GetOriginalLanguageIndex() const;

  
  void UpdateOriginalLanguageIndex(size_t language_index);

  
  size_t GetTargetLanguageIndex() const;

  
  void UpdateTargetLanguageIndex(size_t language_index);

  
  std::string GetLanguageCodeAt(size_t index) const;

  
  base::string16 GetLanguageNameAt(size_t index) const;

  
  std::string GetOriginalLanguageCode() const;

  
  std::string GetTargetLanguageCode() const;

  
  void Translate();

  
  void RevertTranslation();

  
  void TranslationDeclined(bool explicitly_closed);

  
  bool IsLanguageBlocked();

  
  void SetLanguageBlocked(bool value);

  
  bool IsSiteBlacklisted();

  
  void SetSiteBlacklist(bool value);

  
  
  bool ShouldAlwaysTranslate();

  
  
  void SetAlwaysTranslate(bool value);

 private:
  
  
  std::string GetPageHost();

  content::WebContents* web_contents_;

  typedef std::pair<std::string, base::string16> LanguageNamePair;

  
  
  
  
  std::vector<LanguageNamePair> languages_;

  
  size_t original_language_index_;

  
  
  
  
  
  size_t initial_original_language_index_;

  
  size_t target_language_index_;

  
  scoped_ptr<TranslatePrefs> prefs_;

  DISALLOW_COPY_AND_ASSIGN(TranslateUIDelegate);
};

#endif  
