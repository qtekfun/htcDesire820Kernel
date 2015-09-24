// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_RENDERER_SPELLCHECKER_SPELLCHECK_WORDITERATOR_H_
#define CHROME_RENDERER_SPELLCHECKER_SPELLCHECK_WORDITERATOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "third_party/icu/source/common/unicode/ubrk.h"
#include "third_party/icu/source/common/unicode/uscript.h"

class SpellcheckCharAttribute {
 public:
  SpellcheckCharAttribute();
  ~SpellcheckCharAttribute();

  
  
  
  
  void SetDefaultLanguage(const std::string& language);

  
  
  
  
  
  base::string16 GetRuleSet(bool allow_contraction) const;

  
  
  bool OutputChar(UChar c, base::string16* output) const;

 private:
  
  
  
  
  
  void CreateRuleSets(const std::string& language);

  
  
  bool OutputArabic(UChar c, base::string16* output) const;
  bool OutputHangul(UChar c, base::string16* output) const;
  bool OutputHebrew(UChar c, base::string16* output) const;
  bool OutputDefault(UChar c, base::string16* output) const;

  
  
  
  base::string16 ruleset_allow_contraction_;
  base::string16 ruleset_disallow_contraction_;

  
  UScriptCode script_code_;

  DISALLOW_COPY_AND_ASSIGN(SpellcheckCharAttribute);
};

class SpellcheckWordIterator {
 public:
  SpellcheckWordIterator();
  ~SpellcheckWordIterator();

  
  
  
  
  bool Initialize(const SpellcheckCharAttribute* attribute,
                  bool allow_contraction);

  
  bool IsInitialized() const;

  
  
  
  bool SetText(const char16* text, size_t length);

  
  
  
  
  
  
  
  
  
  
  bool GetNextWord(base::string16* word_string,
                   int* word_start,
                   int* word_length);

  
  void Reset();

 private:
  
  
  
  
  
  
  
  bool Normalize(int input_start,
                 int input_length,
                 base::string16* output_string) const;

  
  const char16* text_;

  
  int length_;

  
  int position_;

  
  
  const SpellcheckCharAttribute* attribute_;

  
  UBreakIterator* iterator_;

  DISALLOW_COPY_AND_ASSIGN(SpellcheckWordIterator);
};

#endif  

