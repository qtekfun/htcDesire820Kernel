// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPELLCHECKER_MISSPELLING_H_
#define CHROME_BROWSER_SPELLCHECKER_MISSPELLING_H_

#include <vector>

#include "base/time/time.h"
#include "chrome/browser/spellchecker/spellcheck_action.h"

class Misspelling {
 public:
  Misspelling();
  Misspelling(const base::string16& context,
              size_t location,
              size_t length,
              const std::vector<base::string16>& suggestions,
              uint32 hash);
  ~Misspelling();

  
  
  base::DictionaryValue* Serialize() const;

  
  
  base::string16 GetMisspelledString() const;

  
  base::string16 context;

  
  
  size_t location;

  
  size_t length;

  
  std::vector<base::string16> suggestions;

  
  uint32 hash;

  
  SpellcheckAction action;

  
  base::Time timestamp;
};

#endif  
