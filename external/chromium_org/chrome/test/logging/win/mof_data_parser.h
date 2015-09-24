// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_LOGGING_WIN_MOF_DATA_PARSER_H_
#define CHROME_TEST_LOGGING_WIN_MOF_DATA_PARSER_H_

#include <stddef.h>
#include <windows.h>
#include <wmistr.h>
#include <evntrace.h>

#include "base/basictypes.h"
#include "base/strings/string_piece.h"

namespace logging_win {

class MofDataParser {
 public:
  explicit MofDataParser(const EVENT_TRACE* event);

  bool ReadDWORD(DWORD* value) {
    return ReadPrimitive(value);
  }

  bool ReadInt(int* value) {
    return ReadPrimitive(value);
  }

  bool ReadPointer(intptr_t* value) {
    return ReadPrimitive(value);
  }

  
  
  bool ReadPointerArray(DWORD size, const intptr_t** values) {
    return ReadPrimitiveArray(size, values);
  }

  
  
  template<typename T> bool ReadStructure(const T** value) {
    if (length_ < sizeof(**value))
      return false;
    *value = reinterpret_cast<const T*>(scan_);
    Advance(sizeof(**value));
    return true;
  }

  
  
  
  bool ReadString(base::StringPiece* value);

  bool empty() { return length_ == 0; }

 private:
  void Advance(size_t num_bytes) {
    scan_ += num_bytes;
    length_ -= num_bytes;
  }

  template<typename T> bool ReadPrimitive(T* value) {
    if (length_ < sizeof(*value))
      return false;
    *value = *reinterpret_cast<const T*>(scan_);
    Advance(sizeof(*value));
    return true;
  }

  template<typename T> bool ReadPrimitiveArray(DWORD size, const T** values) {
    if (length_ < sizeof(**values) * size)
      return false;
    *values = reinterpret_cast<const T*>(scan_);
    Advance(sizeof(**values) * size);
    return true;
  }

  const uint8* scan_;
  uint32 length_;
};

}  

#endif  
