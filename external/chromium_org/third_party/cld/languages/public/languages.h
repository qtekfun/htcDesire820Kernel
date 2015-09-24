// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LANGUAGES_PUBLIC_LANGUAGES_H_
#define LANGUAGES_PUBLIC_LANGUAGES_H_



#ifndef SWIG
#include "languages/proto/languages.pb.h"


#else
#include "i18n/languages/internal/languages_proto_wrapper.h"

#endif


const int kNumLanguages = NUM_LANGUAGES;

Language default_language();



inline bool IsValidLanguage(int lang) {
  return ((lang >= 0) && (lang < kNumLanguages));
}

inline bool IS_LANGUAGE_UNKNOWN(Language lang) {
  return lang == TG_UNKNOWN_LANGUAGE || lang == UNKNOWN_LANGUAGE;
}

bool IsCJKLanguage(Language lang);

bool IsChineseLanguage(Language lang);

bool IsNorwegianLanguage(Language lang);

bool IsPortugueseLanguage(Language lang);

bool IsSameLanguage(Language lang1, Language lang2);


// This function returns true if the language is only written right-to-left
// There are languages that can be written in more than one script.
//   - Kurdish and Azeri ('AZERBAIJANI') can be written left-to-right in
//   - Sindhi and Punjabi are written in different scripts, depending on
//   - Kashmiri and Urdu are written either with Arabic or Devanagari script.
// written in right-to-left script.
bool IsRightToLeftLanguage(Language lang);

bool IsMaybeRightToLeftLanguage(Language lang);




const char* LanguageName(Language lang);

const char* LanguageEnumName(Language lang);

const int kMaxLanguageNameSize = 50;

const char* default_language_name();

const char* invalid_language_name();

bool LanguageFromName(const char* lang_name, Language *language);




const char* LanguageCode(Language lang);

const int kMaxLanguageCodeSize = 50;

const char* default_language_code();

const char* invalid_language_code();



const char* LanguageCodeWithDialects(Language lang);

const char* LanguageCodeISO639_1(Language lang);

const char* LanguageCodeISO639_2(Language lang);

bool LanguageFromCode(const char* lang_code, Language *language);


bool LanguageFromCodeOrName(const char* lang_code_or_name,
                            Language* language);

const char* LanguageNameFromCode(const char* language_code);




bool LanguageCodeToUnderscoreForm(const char* lang_code,
                                  char* result,
                                  int result_length);

bool AlwaysPutInExpectedRestrict(const char *tld, Language *expected_language);


#endif  
