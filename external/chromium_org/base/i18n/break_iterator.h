// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_I18N_BREAK_ITERATOR_H_
#define BASE_I18N_BREAK_ITERATOR_H_

#include "base/basictypes.h"
#include "base/i18n/base_i18n_export.h"
#include "base/strings/string16.h"


namespace base {
namespace i18n {

class BASE_I18N_EXPORT BreakIterator {
 public:
  enum BreakType {
    BREAK_WORD,
    BREAK_LINE,
    
    
    
    BREAK_SPACE = BREAK_LINE,
    BREAK_NEWLINE,
    BREAK_CHARACTER,
  };

  
  BreakIterator(const string16& str, BreakType break_type);
  ~BreakIterator();

  
  
  bool Init();

  
  
  
  
  bool Advance();

  
  
  
  
  bool IsWord() const;

  
  
  
  bool IsEndOfWord(size_t position) const;
  bool IsStartOfWord(size_t position) const;

  
  
  
  string16 GetString() const;

  
  size_t prev() const { return prev_; }

  
  
  size_t pos() const { return pos_; }

 private:
  
  
  
  
  void* iter_;

  
  const string16& string_;

  
  BreakType break_type_;

  
  size_t prev_, pos_;

  DISALLOW_COPY_AND_ASSIGN(BreakIterator);
};

}  
}  

#endif  
