// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_SPELLCHECK_RESULT_H_
#define CHROME_COMMON_SPELLCHECK_RESULT_H_

#include "base/strings/string16.h"

struct SpellCheckResult {
  enum Decoration {
    
    SPELLING = 1 << 1,

    
    
    GRAMMAR = 1 << 2,

    
    
    INVISIBLE = 1 << 3,
  };

  explicit SpellCheckResult(
      Decoration d = SPELLING,
      int loc = 0,
      int len = 0,
      const base::string16& rep = base::string16(),
      uint32 h = 0)
      : decoration(d), location(loc), length(len), replacement(rep), hash(h) {
  }

  Decoration decoration;
  int location;
  int length;
  base::string16 replacement;
  uint32 hash;
};

#endif  
