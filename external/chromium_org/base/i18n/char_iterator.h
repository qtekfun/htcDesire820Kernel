// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_I18N_CHAR_ITERATOR_H_
#define BASE_I18N_CHAR_ITERATOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/i18n/base_i18n_export.h"
#include "base/strings/string16.h"


#if defined(OS_WIN)
typedef unsigned char uint8_t;
#endif

namespace base {
namespace i18n {

class BASE_I18N_EXPORT UTF8CharIterator {
 public:
  
  explicit UTF8CharIterator(const std::string* str);
  ~UTF8CharIterator();

  
  
  int32 array_pos() const { return array_pos_; }

  
  
  int32 char_pos() const { return char_pos_; }

  
  int32 get() const { return char_; }

  
  bool end() const { return array_pos_ == len_; }

  
  
  bool Advance();

 private:
  
  const uint8_t* str_;

  
  int32 len_;

  
  int32 array_pos_;

  
  int32 next_pos_;

  
  int32 char_pos_;

  
  int32 char_;

  DISALLOW_COPY_AND_ASSIGN(UTF8CharIterator);
};

class BASE_I18N_EXPORT UTF16CharIterator {
 public:
  
  explicit UTF16CharIterator(const string16* str);
  UTF16CharIterator(const char16* str, size_t str_len);
  ~UTF16CharIterator();

  
  
  int32 array_pos() const { return array_pos_; }

  
  
  int32 char_pos() const { return char_pos_; }

  
  int32 get() const { return char_; }

  
  bool end() const { return array_pos_ == len_; }

  
  
  bool Advance();

 private:
  
  
  void ReadChar();

  
  const char16* str_;

  
  int32 len_;

  
  int32 array_pos_;

  
  int32 next_pos_;

  
  int32 char_pos_;

  
  int32 char_;

  DISALLOW_COPY_AND_ASSIGN(UTF16CharIterator);
};

}  
}  

#endif  
