// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_SPELLCHECKER_SPELLCHECK_PLATFORM_MAC_H_
#define CHROME_BROWSER_SPELLCHECKER_SPELLCHECK_PLATFORM_MAC_H_

#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/strings/string16.h"

struct SpellCheckResult;

namespace content {
class BrowserMessageFilter;
}  

namespace spellcheck_mac {

typedef base::Callback<void(
        const std::vector<SpellCheckResult>& )>
            TextCheckCompleteCallback;

void GetAvailableLanguages(std::vector<std::string>* spellcheck_languages);

bool SpellCheckerAvailable();

bool SpellCheckerProvidesPanel();

bool SpellingPanelVisible();

void ShowSpellingPanel(bool show);

void UpdateSpellingPanelWithMisspelledWord(const base::string16& word);

bool PlatformSupportsLanguage(const std::string& current_language);

void SetLanguage(const std::string& lang_to_set);

bool CheckSpelling(const base::string16& word_to_check, int tag);

void FillSuggestionList(const base::string16& wrong_word,
                        std::vector<base::string16>* optional_suggestions);

void AddWord(const base::string16& word);

void RemoveWord(const base::string16& word);

int GetDocumentTag();

void IgnoreWord(const base::string16& word);

void CloseDocumentWithTag(int tag);

void RequestTextCheck(int document_tag,
                      const base::string16& text,
                      TextCheckCompleteCallback callback);

class SpellcheckerStateInternal;

class ScopedEnglishLanguageForTest {
 public:
  ScopedEnglishLanguageForTest();
  ~ScopedEnglishLanguageForTest();
 private:
  SpellcheckerStateInternal* state_;
};

}  

#endif  
