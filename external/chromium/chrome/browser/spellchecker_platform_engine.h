// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_SPELLCHECKER_PLATFORM_ENGINE_H_
#define CHROME_BROWSER_SPELLCHECKER_PLATFORM_ENGINE_H_
#pragma once

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/string16.h"

class BrowserMessageFilter;

namespace SpellCheckerPlatform {

void GetAvailableLanguages(std::vector<std::string>* spellcheck_languages);

bool SpellCheckerAvailable();

bool SpellCheckerProvidesPanel();

bool SpellingPanelVisible();

void ShowSpellingPanel(bool show);

void UpdateSpellingPanelWithMisspelledWord(const string16& word);

void Init();

bool PlatformSupportsLanguage(const std::string& current_language);

void SetLanguage(const std::string& lang_to_set);

bool CheckSpelling(const string16& word_to_check, int tag);

void FillSuggestionList(const string16& wrong_word,
                        std::vector<string16>* optional_suggestions);

void AddWord(const string16& word);

void RemoveWord(const string16& word);

int GetDocumentTag();

void IgnoreWord(const string16& word);

void CloseDocumentWithTag(int tag);

void RequestTextCheck(int route_id,
                      int identifier,
                      int document_tag,
                      const string16& text,
                      BrowserMessageFilter* destination);

}  

#endif  
