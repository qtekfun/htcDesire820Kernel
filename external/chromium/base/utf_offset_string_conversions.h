// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_UTF_OFFSET_STRING_CONVERSIONS_H_
#define BASE_UTF_OFFSET_STRING_CONVERSIONS_H_
#pragma once

#include <string>
#include <vector>

#include "base/base_api.h"
#include "base/string16.h"

namespace base {
class StringPiece;
}

BASE_API bool UTF8ToWideAndAdjustOffset(const char* src,
                                        size_t src_len,
                                        std::wstring* output,
                                        size_t* offset_for_adjustment);
BASE_API bool UTF8ToWideAndAdjustOffsets(
    const char* src,
    size_t src_len,
    std::wstring* output,
    std::vector<size_t>* offsets_for_adjustment);

BASE_API std::wstring UTF8ToWideAndAdjustOffset(const base::StringPiece& utf8,
                                                size_t* offset_for_adjustment);
BASE_API std::wstring UTF8ToWideAndAdjustOffsets(
    const base::StringPiece& utf8,
    std::vector<size_t>* offsets_for_adjustment);

BASE_API bool UTF16ToWideAndAdjustOffset(const char16* src,
                                         size_t src_len,
                                         std::wstring* output,
                                         size_t* offset_for_adjustment);
BASE_API bool UTF16ToWideAndAdjustOffsets(
    const char16* src,
    size_t src_len,
    std::wstring* output,
    std::vector<size_t>* offsets_for_adjustment);

BASE_API std::wstring UTF16ToWideAndAdjustOffset(const string16& utf16,
                                                 size_t* offset_for_adjustment);
BASE_API std::wstring UTF16ToWideAndAdjustOffsets(
    const string16& utf16,
    std::vector<size_t>* offsets_for_adjustment);

template <typename T>
struct LimitOffset {
  explicit LimitOffset(size_t limit)
    : limit_(limit) {}

  void operator()(size_t& offset) {
    if (offset >= limit_)
      offset = T::npos;
  }

  size_t limit_;
};

struct AdjustOffset {
  
  
  struct Adjustment {
    Adjustment(size_t location, size_t old_length, size_t new_length);

    size_t location;
    size_t old_length;
    size_t new_length;
  };

  typedef std::vector<Adjustment> Adjustments;

  explicit AdjustOffset(const Adjustments& adjustments);
  void operator()(size_t& offset);

  const Adjustments& adjustments_;
};

#endif  
