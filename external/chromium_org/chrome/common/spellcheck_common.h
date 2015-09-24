// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_SPELLCHECK_COMMON_H_
#define CHROME_COMMON_SPELLCHECK_COMMON_H_

#include <set>
#include <string>
#include <vector>

namespace base {
class FilePath;
}

namespace chrome {
namespace spellcheck_common {

static const int kSessionHours = 6;

static const int kContextWordCount = 2;

static const int kFeedbackIntervalSeconds = 1800;  

static const int kMaxSuggestions = 5;

static const int kMaxAutoCorrectWordSize = 8;

static const size_t MAX_SYNCABLE_DICTIONARY_WORDS = 1300;

static const size_t MAX_CUSTOM_DICTIONARY_WORD_BYTES = 99;

typedef std::vector<std::string> WordList;
typedef std::set<std::string> WordSet;

base::FilePath GetVersionedFileName(const std::string& input_language,
                                    const base::FilePath& dict_dir);

std::string GetCorrespondingSpellCheckLanguage(const std::string& language);

void SpellCheckLanguages(std::vector<std::string>* languages);

void GetISOLanguageCountryCodeFromLocale(const std::string& locale,
                                         std::string* language_code,
                                         std::string* country_code);

}  
}  

#endif  
