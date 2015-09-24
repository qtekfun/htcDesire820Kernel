// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_INTEGRATION_DICTIONARY_HELPER_H_
#define CHROME_BROWSER_SYNC_TEST_INTEGRATION_DICTIONARY_HELPER_H_

#include <string>

#include "base/command_line.h"

class SpellcheckCustomDictionary;

namespace dictionary_helper {

void LoadDictionaries();

size_t GetDictionarySize(int index);

size_t GetVerifierDictionarySize();

bool DictionariesMatch();

bool DictionaryMatchesVerifier(int index);

bool AddWord(int index, const std::string& word);

bool RemoveWord(int index, const std::string& word);

}  

#endif  
