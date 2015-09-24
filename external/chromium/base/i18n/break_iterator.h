// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_I18N_BREAK_ITERATOR_H_
#define BASE_I18N_BREAK_ITERATOR_H_
#pragma once

#include "base/basictypes.h"
#include "base/string16.h"


namespace base {

class BreakIterator {
 public:
  enum BreakType {
    BREAK_WORD,
    BREAK_LINE,
    
    
    
    BREAK_SPACE = BREAK_LINE,
    BREAK_NEWLINE,
  };

  
  BreakIterator(const string16* str, BreakType break_type);
  ~BreakIterator();

  
  
  bool Init();

  
  
  size_t pos() const { return pos_; }

  
  size_t prev() const { return prev_; }

  
  
  
  
  bool Advance();

  
  
  
  
  bool IsWord() const;

  
  
  
  string16 GetString() const;

 private:
  
  
  
  
  void* iter_;

  
  const string16* string_;

  
  BreakType break_type_;

  
  size_t prev_, pos_;

  DISALLOW_COPY_AND_ASSIGN(BreakIterator);
};

}  

#endif  
