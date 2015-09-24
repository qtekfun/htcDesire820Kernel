// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPELLCHECKER_SPELLCHECK_ACTION_H_
#define CHROME_BROWSER_SPELLCHECKER_SPELLCHECK_ACTION_H_

#include "base/strings/string16.h"

namespace base {
class DictionaryValue;
}

class SpellcheckAction {
 public:
  
  enum SpellcheckActionType {
    
    
    TYPE_ADD_TO_DICT,
    
    
    
    TYPE_IGNORE,
    
    
    TYPE_IN_DICTIONARY,
    
    
    TYPE_MANUALLY_CORRECTED,
    
    
    
    TYPE_NO_ACTION,
    
    
    TYPE_PENDING,
    
    
    
    TYPE_PENDING_IGNORE,
    
    
    TYPE_SELECT,
  };

  SpellcheckAction();
  SpellcheckAction(SpellcheckActionType type, int index, base::string16 value);
  ~SpellcheckAction();

  
  
  bool IsFinal() const;

  
  
  
  void Finalize();

  
  
  base::DictionaryValue* Serialize() const;

  
  SpellcheckActionType type;

  
  int index;

  
  base::string16 value;
};

#endif  
