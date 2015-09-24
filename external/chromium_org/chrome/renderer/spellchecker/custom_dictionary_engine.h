// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_SPELLCHECKER_CUSTOM_DICTIONARY_ENGINE_H_
#define CHROME_RENDERER_SPELLCHECKER_CUSTOM_DICTIONARY_ENGINE_H_

#include <set>
#include <string>
#include <vector>

#include "base/strings/string16.h"

class CustomDictionaryEngine {
 public:
  CustomDictionaryEngine();
  ~CustomDictionaryEngine();

  
  void Init(const std::set<std::string>& words);

  
  
  
  bool SpellCheckWord(const base::string16& text,
                      int misspelling_start,
                      int misspelling_len);

  
  void OnCustomDictionaryChanged(
      const std::vector<std::string>& words_added,
      const std::vector<std::string>& words_removed);

 private:
  
  std::set<base::string16> dictionary_;

  DISALLOW_COPY_AND_ASSIGN(CustomDictionaryEngine);
};

#endif  
