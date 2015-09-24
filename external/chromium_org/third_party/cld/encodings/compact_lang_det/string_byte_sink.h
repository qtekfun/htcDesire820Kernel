// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ENCODINGS_COMPACT_LANG_DET_STRING_BYTE_SINK_H_
#define ENCODINGS_COMPACT_LANG_DET_STRING_BYTE_SINK_H_

#include <string>

#include <unicode/unistr.h>

class StringByteSink : public icu::ByteSink {
 public:
  
  explicit StringByteSink(std::string* dest);
  virtual ~StringByteSink();

  virtual void Append(const char* data, int32_t n);

 private:
  std::string* const dest_;
};

#endif  
