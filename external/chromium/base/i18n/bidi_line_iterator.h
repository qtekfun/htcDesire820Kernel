// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_I18N_BIDI_LINE_ITERATOR_H_
#define BASE_I18N_BIDI_LINE_ITERATOR_H_
#pragma once

#include "unicode/ubidi.h"

#include "base/basictypes.h"
#include "base/string16.h"

namespace base {
namespace i18n {

class BiDiLineIterator {
 public:
  BiDiLineIterator();
  ~BiDiLineIterator();

  
  
  bool Open(const string16& text, bool right_to_left, bool url);

  
  int CountRuns();

  
  UBiDiDirection GetVisualRun(int index, int* start, int* length);

  
  void GetLogicalRun(int start, int* end, UBiDiLevel* level);

 private:
  UBiDi* bidi_;

  DISALLOW_COPY_AND_ASSIGN(BiDiLineIterator);
};

}  
}  

#endif  
