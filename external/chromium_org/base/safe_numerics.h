// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SAFE_NUMERICS_H_
#define BASE_SAFE_NUMERICS_H_

#include <limits>

#include "base/logging.h"

namespace base {
namespace internal {

template <bool SameSize, bool DestLarger,
          bool DestIsSigned, bool SourceIsSigned>
struct IsValidNumericCastImpl;

#define BASE_NUMERIC_CAST_CASE_SPECIALIZATION(A, B, C, D, Code) \
template <> struct IsValidNumericCastImpl<A, B, C, D> { \
  template <class Source, class DestBounds> static inline bool Test( \
      Source source, DestBounds min, DestBounds max) { \
    return Code; \
  } \
}

#define BASE_NUMERIC_CAST_CASE_SAME_SIZE(DestSigned, SourceSigned, Code) \
  BASE_NUMERIC_CAST_CASE_SPECIALIZATION( \
      true, true, DestSigned, SourceSigned, Code); \
  BASE_NUMERIC_CAST_CASE_SPECIALIZATION( \
      true, false, DestSigned, SourceSigned, Code)

#define BASE_NUMERIC_CAST_CASE_SOURCE_LARGER(DestSigned, SourceSigned, Code) \
  BASE_NUMERIC_CAST_CASE_SPECIALIZATION( \
      false, false, DestSigned, SourceSigned, Code); \

#define BASE_NUMERIC_CAST_CASE_DEST_LARGER(DestSigned, SourceSigned, Code) \
  BASE_NUMERIC_CAST_CASE_SPECIALIZATION( \
      false, true, DestSigned, SourceSigned, Code); \




BASE_NUMERIC_CAST_CASE_SAME_SIZE(true, true, true);
BASE_NUMERIC_CAST_CASE_SAME_SIZE(false, false, true);
BASE_NUMERIC_CAST_CASE_SAME_SIZE(false, true, source >= 0);
BASE_NUMERIC_CAST_CASE_SAME_SIZE(true, false,
                                 source <= static_cast<Source>(max));



BASE_NUMERIC_CAST_CASE_SOURCE_LARGER(false, false, source <= max);
BASE_NUMERIC_CAST_CASE_SOURCE_LARGER(true, true,
                                     source >= min && source <= max);
BASE_NUMERIC_CAST_CASE_SOURCE_LARGER(false, true,
                                     source >= 0 && source <= max);
BASE_NUMERIC_CAST_CASE_SOURCE_LARGER(true, false,
                                     source <= static_cast<Source>(max));



BASE_NUMERIC_CAST_CASE_DEST_LARGER(false, false, true);
BASE_NUMERIC_CAST_CASE_DEST_LARGER(true, true, true);
BASE_NUMERIC_CAST_CASE_DEST_LARGER(false, true, source >= 0);
BASE_NUMERIC_CAST_CASE_DEST_LARGER(true, false, true);

#undef BASE_NUMERIC_CAST_CASE_SPECIALIZATION
#undef BASE_NUMERIC_CAST_CASE_SAME_SIZE
#undef BASE_NUMERIC_CAST_CASE_SOURCE_LARGER
#undef BASE_NUMERIC_CAST_CASE_DEST_LARGER


template <class Dest, class Source>
inline bool IsValidNumericCast(Source source) {
  typedef std::numeric_limits<Source> SourceLimits;
  typedef std::numeric_limits<Dest> DestLimits;
  COMPILE_ASSERT(SourceLimits::is_specialized, argument_must_be_numeric);
  COMPILE_ASSERT(SourceLimits::is_integer, argument_must_be_integral);
  COMPILE_ASSERT(DestLimits::is_specialized, result_must_be_numeric);
  COMPILE_ASSERT(DestLimits::is_integer, result_must_be_integral);

  return IsValidNumericCastImpl<
      sizeof(Dest) == sizeof(Source),
      (sizeof(Dest) > sizeof(Source)),
      DestLimits::is_signed,
      SourceLimits::is_signed>::Test(
          source,
          DestLimits::min(),
          DestLimits::max());
}

}  

template <class Dest, class Source>
inline Dest checked_numeric_cast(Source source) {
  CHECK(internal::IsValidNumericCast<Dest>(source));
  return static_cast<Dest>(source);
}

}  

#endif  
