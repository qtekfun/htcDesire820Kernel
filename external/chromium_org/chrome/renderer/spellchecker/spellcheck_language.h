// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_SPELLCHECKER_SPELLCHECK_LANGUAGE_H_
#define CHROME_RENDERER_SPELLCHECKER_SPELLCHECK_LANGUAGE_H_

#include <queue>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/platform_file.h"
#include "base/strings/string16.h"
#include "chrome/renderer/spellchecker/spellcheck_worditerator.h"

class SpellingEngine;

class SpellcheckLanguage {
 public:
  SpellcheckLanguage();
  ~SpellcheckLanguage();

  void Init(base::PlatformFile file, const std::string& language);

  
  
  
  
  
  
  
  
  
  
  
  bool SpellCheckWord(const char16* in_word,
                      int in_word_len,
                      int tag,
                      int* misspelling_start,
                      int* misspelling_len,
                      std::vector<base::string16>* optional_suggestions);

  
  bool InitializeIfNeeded();

  
  bool IsEnabled();

 private:
  friend class SpellCheckTest;

  
  
  bool IsValidContraction(const base::string16& word, int tag);

  
  
  SpellcheckCharAttribute character_attributes_;

  
  
  
  
  
  SpellcheckWordIterator text_iterator_;
  SpellcheckWordIterator contraction_iterator_;

  
  
  scoped_ptr<SpellingEngine> platform_spelling_engine_;

  DISALLOW_COPY_AND_ASSIGN(SpellcheckLanguage);
};

#endif  
