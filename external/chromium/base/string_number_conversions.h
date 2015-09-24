// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_STRING_NUMBER_CONVERSIONS_H_
#define BASE_STRING_NUMBER_CONVERSIONS_H_

#include <string>
#include <vector>

#include "base/base_api.h"
#include "base/basictypes.h"
#include "base/string16.h"


namespace base {


BASE_API std::string IntToString(int value);
BASE_API string16 IntToString16(int value);

BASE_API std::string UintToString(unsigned value);
BASE_API string16 UintToString16(unsigned value);

BASE_API std::string Int64ToString(int64 value);
BASE_API string16 Int64ToString16(int64 value);

BASE_API std::string Uint64ToString(uint64 value);
BASE_API string16 Uint64ToString16(uint64 value);

BASE_API std::string DoubleToString(double value);


BASE_API bool StringToInt(const std::string& input, int* output);
BASE_API bool StringToInt(std::string::const_iterator begin,
                          std::string::const_iterator end,
                          int* output);
BASE_API bool StringToInt(const char* begin, const char* end, int* output);

BASE_API bool StringToInt(const string16& input, int* output);
BASE_API bool StringToInt(string16::const_iterator begin,
                          string16::const_iterator end,
                          int* output);
BASE_API bool StringToInt(const char16* begin, const char16* end, int* output);

BASE_API bool StringToInt64(const std::string& input, int64* output);
BASE_API bool StringToInt64(std::string::const_iterator begin,
                            std::string::const_iterator end,
                            int64* output);
BASE_API bool StringToInt64(const char* begin, const char* end, int64* output);

BASE_API bool StringToInt64(const string16& input, int64* output);
BASE_API bool StringToInt64(string16::const_iterator begin,
                            string16::const_iterator end,
                            int64* output);
BASE_API bool StringToInt64(const char16* begin, const char16* end,
                            int64* output);

BASE_API bool StringToDouble(const std::string& input, double* output);


// within reasonable limits since it's written with trusted data in mind.  If
BASE_API std::string HexEncode(const void* bytes, size_t size);

BASE_API bool HexStringToInt(const std::string& input, int* output);
BASE_API bool HexStringToInt(std::string::const_iterator begin,
                             std::string::const_iterator end,
                             int* output);
BASE_API bool HexStringToInt(const char* begin, const char* end, int* output);

BASE_API bool HexStringToBytes(const std::string& input,
                               std::vector<uint8>* output);

}  

#endif  

