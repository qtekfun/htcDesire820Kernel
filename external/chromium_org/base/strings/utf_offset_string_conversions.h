// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_STRINGS_UTF_OFFSET_STRING_CONVERSIONS_H_
#define BASE_STRINGS_UTF_OFFSET_STRING_CONVERSIONS_H_

#include <string>
#include <vector>

#include "base/base_export.h"
#include "base/strings/string16.h"
#include "base/strings/string_piece.h"

namespace base {

BASE_EXPORT bool UTF8ToUTF16AndAdjustOffset(const char* src,
                                            size_t src_len,
                                            string16* output,
                                            size_t* offset_for_adjustment);
BASE_EXPORT bool UTF8ToUTF16AndAdjustOffsets(
    const char* src,
    size_t src_len,
    string16* output,
    std::vector<size_t>* offsets_for_adjustment);

BASE_EXPORT string16 UTF8ToUTF16AndAdjustOffset(const base::StringPiece& utf8,
                                                size_t* offset_for_adjustment);
BASE_EXPORT string16 UTF8ToUTF16AndAdjustOffsets(
    const base::StringPiece& utf8,
    std::vector<size_t>* offsets_for_adjustment);

BASE_EXPORT std::string UTF16ToUTF8AndAdjustOffset(
    const base::StringPiece16& utf16,
    size_t* offset_for_adjustment);
BASE_EXPORT std::string UTF16ToUTF8AndAdjustOffsets(
    const base::StringPiece16& utf16,
    std::vector<size_t>* offsets_for_adjustment);

template <typename T>
struct LimitOffset {
  explicit LimitOffset(size_t limit)
    : limit_(limit) {}

  void operator()(size_t& offset) {
    if (offset > limit_)
      offset = T::npos;
  }

  size_t limit_;
};

class BASE_EXPORT OffsetAdjuster {
 public:
  struct BASE_EXPORT Adjustment {
    Adjustment(size_t original_offset,
               size_t original_length,
               size_t output_length);

    size_t original_offset;
    size_t original_length;
    size_t output_length;
  };

  explicit OffsetAdjuster(std::vector<size_t>* offsets_for_adjustment);
  ~OffsetAdjuster();

  void Add(const Adjustment& adjustment);

 private:
  void AdjustOffset(std::vector<size_t>::iterator offset);

  std::vector<size_t>* offsets_for_adjustment_;
  std::vector<Adjustment> adjustments_;
};

}  

#endif  
