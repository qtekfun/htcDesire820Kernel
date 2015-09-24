// Copyright 2008 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_VCDIFF_OUTPUT_STRING_CROPE_H_
#define OPEN_VCDIFF_OUTPUT_STRING_CROPE_H_
#include <config.h>
#ifdef HAVE_EXT_ROPE
#include <ext/rope>
#include "google/output_string.h"

namespace open_vcdiff {


template <>
void OutputString<__gnu_cxx::crope>::ReserveAdditionalBytes(
    size_t ) { }

typedef OutputString<__gnu_cxx::crope> OutputCrope;

}  
#endif  
#endif  